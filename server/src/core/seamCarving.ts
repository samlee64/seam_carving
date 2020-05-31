import { execFile } from "child_process";
import { map } from "bluebird";
import config from "../config";
import { GrowParams } from "../types/seamCarving";
import { outputPaths, OutputPath, s3Path } from "./utils";
import { uploadFile } from "../aws/s3";

export function grow(params: GrowParams): void {
  const imageName = params.imageName;

  const args: string[] = ["grow", imageName, params.addWidth.toString(), params.addHeight.toString(), params.numSteps.toString(), params.showIntermediateSteps.toString()];

  console.log("Running with these args", args)

  console.log("starting exec");
  execFile(config.executablePath, args, async (error, stdout, stderr) => {
    if (error) {
      console.error("stderr", stderr);
      console.error("error", error);
      throw error;
    }

    console.log("stdout", stdout);

    console.log("seamCarving, uploading to s3");
    await map(outputPaths(imageName), async (outputPath: OutputPath) => {
      console.log(outputPath);
      const s3 = s3Path(imageName, outputPath.fileName);

      return uploadFile(outputPath.path, s3);
    });

    console.log("seamCarving, finished uploading to s3");
  });
}
