import * as path from "path";
import { map } from "bluebird";
import { execFile } from "child_process";
import config from "../config";
import { Connection } from "../db";
import { Status } from "../types/schema";
import { Routine, RoutineParams } from "../types/seamCarving";
import { updateStatus, insertExecution } from "../store/executions";
import { uploadFile } from "../aws/s3";

interface OutputPath {
  fileName: string;
  path: string;
}

function outputPaths(imageName: string, routine: Routine): OutputPath[] {
  //Returns the location of the newly written png and gif files
  let fileNames: string[];

  switch (routine) {
    case Routine.Grow:
      fileNames = ["mask.gif", "energy-map.gif", "mid.gif", "output.png"];
      break;
    case Routine.ContentAmplification:
      fileNames = ["reduce.gif", "output.png"];
      break;
    case Routine.RemoveObject:
      fileNames = [];
      break;
  }

  return fileNames.map((fileName: string) => {
    return {
      fileName,
      path: path.join(config.dataDir, "output", imageName, fileName),
    };
  });
}

function s3Path(routine: Routine, imageName: string, fileName: string): string {
  return path.join("output", routine, imageName, fileName);
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
    if (error) {
      await updateStatus(conn, executionId, Status.Error);
      console.error("stderr", stderr);
      console.error("error", error);
      throw error;
    }
    await updateStatus(conn, executionId, Status.Uploading);

    console.log("stdout", stdout);

    console.log("seamCarving, uploading to s3");
    try {
      await map(
        outputPaths(params.imageName, routine),

        async (outputPath: OutputPath) => {
          console.log(outputPath);
          const s3 = s3Path(routine, params.imageName, outputPath.fileName);

          return uploadFile(outputPath.path, s3);
        }
      );
    } catch (e) {
      await updateStatus(conn, executionId, Status.Error);
      console.error(e);
      throw e;
    }

    await updateStatus(conn, executionId, Status.Done);

    console.log("seamCarving, finished uploading to s3");
  });

  return executionId;
}
