import { execFile } from "child_process";
import { map } from "bluebird";
import config from "../config";
import { GrowParams } from "../types/seamCarving";
import { outputPaths, OutputPath, s3Path } from "./utils";
import { uploadFile } from "../aws/s3";
import { insertExecution, updateStatus } from "../store/executions";
import { Connection } from "../db";
import { Status } from "../types/schema";

export async function grow(
  conn: Connection,
  params: GrowParams
): Promise<string> {
  const imageName = params.imageName;

  const args: string[] = [
    "grow",
    imageName,
    params.addWidth.toString(),
    params.addHeight.toString(),
    params.numSteps.toString(),
    params.showIntermediateSteps.toString(),
  ];

  const executionId = await insertExecution(conn, {
    imageName,
    routine: "grow",
    params,
  });

  console.log("Running with these args", args);
  console.log("starting exec");
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
      await map(outputPaths(imageName), async (outputPath: OutputPath) => {
        console.log(outputPath);
        const s3 = s3Path(imageName, outputPath.fileName);

        return uploadFile(outputPath.path, s3);
      });
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
