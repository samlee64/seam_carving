import * as fs from "fs";
import * as path from "path";
import { S3 } from "aws-sdk";
import config from "../config";
import { IMAGE_BUCKET } from "./constants";
import { InputImages } from "../types/s3";

const s3 = new S3({
  region: "us-west-2",
  accessKeyId: config.aws.accessKey,
  secretAccessKey: config.aws.secretAccessKey,
});

export async function uploadFile(filePath: string, key: string): Promise<void> {
  const dataStream = fs.createReadStream(filePath);

  const params = {
    Body: dataStream,
    Bucket: IMAGE_BUCKET,
    Key: key,
    ACL: "public-read",
  };

  await s3.putObject(params).promise();
}

export async function listInputImages(): Promise<InputImages> {
  const params = { Bucket: IMAGE_BUCKET, Prefix: "defaults" };
  const resp = await s3.listObjectsV2(params).promise();

  if (!resp.Contents) return { keys: [], bucket: IMAGE_BUCKET };

  const filtered: string[] = resp.Contents.map((i) => i.Key).filter(
    (element) => element !== undefined
  ) as string[];

  return { bucket: IMAGE_BUCKET, keys: filtered };
}

export async function downloadDefaultImage(imageName: string): Promise<string> {
  const key = path.join("defaults", imageName + ".png");
  const params = { Bucket: IMAGE_BUCKET, Key: key };

  const dirPath = "data/input";
  await fs.promises.mkdir(dirPath, { recursive: true });
  const filePath = path.join(dirPath, imageName + ".png");

  try {
    await fs.promises.access(filePath);

    return filePath;
  } catch (e) {
    return new Promise((resolve, reject) => {
      const file = fs.createWriteStream(filePath);
      const s3Stream = s3.getObject(params).createReadStream();

      s3Stream.on("error", reject);

      file.on("error", reject);
      file.on("close", () => {
        resolve(filePath);
      });

      s3Stream.pipe(file);
    });
  }
}
