import * as fs from 'fs';
//import * as utils from 'utils';
import { S3 } from 'aws-sdk';
import config from '../config';
import { IMAGE_BUCKET } from './constants';
import { InputImages } from '../types/s3';

const s3 = new S3({
  region: 'us-west-2',
  accessKeyId: config.aws.accessKey,
  secretAccessKey: config.aws.secretAccessKey,
});

export async function uploadFile(filePath: string, key: string): Promise<void> {
  //TODO maybe add tagging for user generated content
  //  const readFile = utils.promisify(fs.readFile);
  //
  console.log('starting file read/upload');
  const data = fs.readFileSync(filePath);

  const params = {
    Body: data,
    Bucket: IMAGE_BUCKET,
    Key: key,
    ACL: 'public-read',
  };

  if (config.env === 'dev') {
    console.log('env dev, skipping upload');
    return;
  }

  console.log('s3, begin upload', filePath, key);
  await s3.putObject(params).promise();
  console.log('s3, finished upload', filePath, key);
}

export async function listInputImages(): Promise<InputImages> {
  const params = { Bucket: IMAGE_BUCKET, Prefix: 'defaults' };

  const resp = await s3.listObjectsV2(params).promise();
  if (!resp.Contents) return { keys: [], bucket: IMAGE_BUCKET };

  const filtered: string[] = resp.Contents.map((i) => i.Key).filter(
    (element) => element !== undefined,
  ) as string[];

  return { keys: filtered, bucket: IMAGE_BUCKET };
}
