import { GrowParams, ContentAmplificationParams, RemoveObjectParams, Routine } from '../types/seamCarving';
import { execFileWithUpload } from './utils';
import { Connection } from '../db';
import { createMask } from './mask';

export async function grow(conn: Connection, params: GrowParams): Promise<string> {
  const imageName = params.imageName;

  const args: string[] = [
    Routine.Grow,
    imageName,
    params.addWidth.toString(),
    params.addHeight.toString(),
    params.numSteps.toString(),
    params.showIntermediateSteps.toString(),
  ];

  const executionId = await execFileWithUpload(conn, Routine.Grow, params, args);
  return executionId;
}

export async function contentAmplification(
  conn: Connection,
  params: ContentAmplificationParams,
): Promise<string> {
  const args: string[] = [
    Routine.ContentAmplification,
    params.imageName,
    params.factor.toString(),
    params.showIntermediateSteps.toString(),
  ];

  console.log('Running content amplification with these args', args);
  console.log('Starting exec');
  const executionId = await execFileWithUpload(conn, Routine.ContentAmplification, params, args);

  return executionId;
}

export async function removeObject(conn: Connection, params: RemoveObjectParams): Promise<string> {
  //I need to add paths for destroy and protect regions
  ////Create mthe masks
  const destroyMaskPath = createMask(
    {
      width: params.imageWidth,
      height: params.imageHeight,
      data: params.markings.destroy,
    },
    params.imageName,
    'destroy.png',
  );
  const protectMaskPath = createMask(
    { width: params.imageWidth, height: params.imageHeight, data: params.markings.protect },
    params.imageName,
    'protect.png',
  );

  const args: string[] = [
    Routine.RemoveObject,
    params.imageName,
    destroyMaskPath,
    protectMaskPath,
    params.lockRatio.toString(),
    params.onlyHorizontal.toString(),
    params.showIntermediateSteps.toString(),
  ];

  console.log('Running removeObject with these args', args);
  console.log('Starting exec');

  const executionId = await execFileWithUpload(conn, Routine.RemoveObject, params, args);
  return executionId;
}
