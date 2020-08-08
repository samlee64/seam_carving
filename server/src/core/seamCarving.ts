import * as fs from "fs";
import {
  GrowParams,
  ContentAmplificationParams,
  RemoveObjectParams,
  Routine,
} from "../types/seamCarving";
import { execFileWithUpload, getOutputPath } from "./utils";
import { createMask } from "./mask";
import { downloadDefaultImage } from "../aws/s3";
import { Connection } from "../db";

export async function grow(
  conn: Connection,
  params: GrowParams
): Promise<string> {
  const routine = Routine.Grow;
  const inputImagePath = await downloadDefaultImage(params.imageName);
  const outputPath = getOutputPath(params.imageName, routine);
  await fs.promises.mkdir(outputPath, { recursive: true });

  const args: string[] = [
    routine,
    inputImagePath,
    outputPath,
    params.addWidth.toString(),
    params.addHeight.toString(),
    params.numSteps.toString(),
    params.showIntermediateSteps.toString(),
  ];

  console.log("Running grow with these args", args);
  const executionId = await execFileWithUpload(conn, routine, params, args);
  return executionId;
}

export async function contentAmplification(
  conn: Connection,
  params: ContentAmplificationParams
): Promise<string> {
  const routine = Routine.ContentAmplification;
  const inputImagePath = await downloadDefaultImage(params.imageName);
  const outputPath = getOutputPath(params.imageName, routine);
  await fs.promises.mkdir(outputPath, { recursive: true });

  const args: string[] = [
    routine,
    inputImagePath,
    outputPath,
    params.factor.toString(),
    params.showIntermediateSteps.toString(),
  ];

  console.log("Running content amplification with these args", args);
  const executionId = await execFileWithUpload(conn, routine, params, args);

  return executionId;
}

export async function removeObject(
  conn: Connection,
  params: RemoveObjectParams
): Promise<string> {
  const routine = Routine.RemoveObject;
  const inputImagePath = await downloadDefaultImage(params.imageName);
  const outputPath = getOutputPath(params.imageName, routine);
  await fs.promises.mkdir(outputPath, { recursive: true });

  const destroyMaskPath = await createMask(
    {
      width: params.imageWidth,
      height: params.imageHeight,
      data: params.markings.destroy,
    },
    params.imageName,
    "destroy.png"
  );

  const protectMaskPath = await createMask(
    {
      width: params.imageWidth,
      height: params.imageHeight,
      data: params.markings.protect,
    },
    params.imageName,
    "protect.png"
  );

  const args: string[] = [
    routine,
    inputImagePath,
    outputPath,
    destroyMaskPath,
    protectMaskPath,
    params.lockRatio.toString(),
    params.onlyHorizontal.toString(),
    params.onlyVertical.toString(),
    params.showIntermediateSteps.toString(),
  ];

  console.log("Running removeObject with these args", args);
  const executionId = await execFileWithUpload(conn, routine, params, args);
  return executionId;
}
