import * as path from "path";
import { map } from "bluebird";
import { execFile } from "child_process";
import config from "../config";
import { Connection } from "../db";
import { Status } from "../types/schema";
import { Routine, RoutineParams } from "../types/seamCarving";
import {
  updateStatus,
  insertExecution,
  updateFileLocations,
} from "../store/executions";
import { uploadFile } from "../aws/s3";

interface Outputs {
  fileNames: string[];
  filePaths: string[];
}

function getOutputFiles(imageName: string, routine: Routine): Outputs {
  if (config.outputs[routine] === undefined) {
    throw new Error(
      `config does not have specified output files for Routine: ${routine}`
    );
  }

  const outputPath = getOutputPath(imageName, routine);

  const fileNames: string[] = config.outputs[routine];
  const filePaths = fileNames.map((fileName: string) => {
    return path.join(outputPath, fileName);
  });

  return { fileNames, filePaths };
}

function getS3Key(
  imageName: string,
  routine: Routine,
  executionId: string,
  fileName: string
): string {
  const outputPath = getOutputPath(imageName, routine);
  return path.join(outputPath, executionId, fileName);
}

export function getOutputPath(imageName: string, routine: Routine): string {
  return path.join(config.dataDir, "output", routine, imageName) + "/";
}

export async function execFileWithUpload(
  conn: Connection,
  routine: Routine,
  params: RoutineParams,
  args: string[]
): Promise<string> {
  const executionId = await insertExecution(conn, {
    imageName: params.imageName,
    routine: routine,
    params: params,
  });

  execFile(config.executablePath, args, async (error, stdout, stderr) => {
    console.error("stderr", stderr);
    console.log("stdout", stdout);

    if (error) {
      console.error("error", error);
      await updateStatus(conn, executionId, Status.Error);
      return;
    }

    await updateStatus(conn, executionId, Status.Uploading);
    console.log("Uploading to s3");

    const outputs: Outputs = getOutputFiles(params.imageName, routine);

    try {
      const s3Keys = await map(outputs.filePaths, async (filePath: string) => {
        const fileName = path.basename(filePath);
        const s3Key = getS3Key(
          params.imageName,
          routine,
          executionId,
          fileName
        );

        await uploadFile(filePath, s3Key);
        return s3Key;
      });

      await updateFileLocations(conn, executionId, s3Keys, outputs.fileNames);
    } catch (e) {
      await updateStatus(conn, executionId, Status.Error);
      throw e;
    }

    console.log("Finished uploading to s3");
    await updateStatus(conn, executionId, Status.Done);
  });

  return executionId;
}
