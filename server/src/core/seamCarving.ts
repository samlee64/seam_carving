import {
  GrowParams,
  ContentAmplificationParams,
  Routine,
} from "../types/seamCarving";
import { execFileWithUpload } from "./utils";
import { Connection } from "../db";

export async function grow(
  conn: Connection,
  params: GrowParams
): Promise<string> {
  const imageName = params.imageName;

  const args: string[] = [
    Routine.Grow,
    imageName,
    params.addWidth.toString(),
    params.addHeight.toString(),
    params.numSteps.toString(),
    params.showIntermediateSteps.toString(),
  ];

  const executionId = await execFileWithUpload(
    conn,
    Routine.Grow,
    params,
    args
  );
  return executionId;
}

export async function contentAmplification(
  conn: Connection,
  params: ContentAmplificationParams
): Promise<string> {
  const args: string[] = [
    Routine.ContentAmplification,
    params.imageName,
    params.factor.toString(),
    params.showIntermediateSteps.toString(),
  ];

  console.log("Running content amplification with these args", args);
  console.log("Starting exec");
  const executionId = await execFileWithUpload(
    conn,
    Routine.ContentAmplification,
    params,
    args
  );

  return executionId;
}
