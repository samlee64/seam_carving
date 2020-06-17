import * as path from 'path';
import { map } from 'bluebird';
import { execFile } from 'child_process';
import config from '../config';
import { Connection } from '../db';
import { Status } from '../types/schema';
import { Routine, RoutineParams } from '../types/seamCarving';
import { updateStatus, insertExecution, updateFileLocations } from '../store/executions';
import { uploadFile } from '../aws/s3';

interface OutputPath {
  path: string;
  s3Key: string;
}

interface Outputs {
  fileNames: string[];
  outputs: OutputPath[];
}

function outputs(imageName: string, routine: Routine, executionId: string): Outputs {
  let fileNames: string[];

  switch (routine) {
    case Routine.Grow:
      fileNames = ['mask.gif', 'energy-map.gif', 'mid.gif', 'output.png'];
      break;
    case Routine.ContentAmplification:
      fileNames = ['reduce.gif', 'output.png'];
      break;
    case Routine.RemoveObject:
      fileNames = [
        'combined-mask.png',
        'energy-map.gif',
        'output.png',
        'mid.gif',
        'protect-destroy-areas.png',
      ];
      break;
  }

  const outputs = fileNames.map((fileName: string) => {
    return {
      path: path.join(config.dataDir, 'output', routine, imageName, fileName),
      s3Key: path.join('output', routine, imageName, executionId, fileName),
    };
  });

  return { fileNames, outputs };
}

export async function execFileWithUpload(
  conn: Connection,
  routine: Routine,
  params: RoutineParams,
  args: string[],
): Promise<string> {
  const executionId = await insertExecution(conn, {
    imageName: params.imageName,
    routine: routine,
    params: params,
  });

  execFile(config.executablePath, args, async (error, stdout, stderr) => {
    console.error('stderr', stderr);
    console.log('stdout', stdout);

    if (error) {
      await updateStatus(conn, executionId, Status.Error);
      console.error('error', error);
      return;
    }

    await updateStatus(conn, executionId, Status.Uploading);

    console.log('seamCarving, uploading to s3');
    try {
      const outputPaths: Outputs = outputs(params.imageName, routine, executionId);

      await map(outputPaths.outputs, async (outputPath: OutputPath) => {
        await uploadFile(outputPath.path, outputPath.s3Key);
      });

      const s3Keys: string[] = outputPaths.outputs.map((path) => path.s3Key);

      //will also assult with metadata;
      await updateFileLocations(conn, executionId, s3Keys, outputPaths.fileNames);
    } catch (e) {
      await updateStatus(conn, executionId, Status.Error);
      console.error(e);
      throw e;
    }

    await updateStatus(conn, executionId, Status.Done);

    console.log('seamCarving, finished uploading to s3');
  });

  return executionId;
}
