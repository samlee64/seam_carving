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
  updateS3Keys,
} from "../store/executions";
import { uploadFile } from "../aws/s3";

interface OutputPath {
  path: string;
  s3Key: string;
}

//dataDir + output + routine + imageName + filename
function outputPaths(imageName: string, routine: Routine): OutputPath[] {
  let fileNames: string[];

  switch (routine) {
    case Routine.Grow:
      fileNames = ["mask.gif", "energy-map.gif", "mid.gif", "output.png"];
      break;
    case Routine.ContentAmplification:
      fileNames = ["reduce.gif", "output.png"];
      break;
    case Routine.RemoveObject:
      fileNames = [
        "combined-mask.png",
        "energy-map.gif",
        "output.png",
        "mid.gif",
        "output.png",
        "remove.gif",
        "protect-destroy-areas.png",
      ];
      break;
  }

  return fileNames.map((fileName: string) => {
    return {
      path: path.join(config.dataDir, "output", routine, imageName, fileName),
      s3Key: path.join("output", routine, imageName, fileName),
    };
  });
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
      return;
    }

    await updateStatus(conn, executionId, Status.Uploading);

    console.log("stdout", stdout);
    console.log("seamCarving, uploading to s3");
    try {
      const paths: OutputPath[] = outputPaths(params.imageName, routine);

      await map(paths, async (outputPath: OutputPath) => {
        await uploadFile(outputPath.path, outputPath.s3Key);
      });

      const s3Keys: string[] = paths.map((path) => path.s3Key);
      await updateS3Keys(conn, executionId, s3Keys);
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
