import * as fs from "fs";
import { S3 } from "aws-sdk";
import config from "../config";
import { IMAGE_BUCKET } from "./constants";

const s3 = new S3({
  region: "us-west-2",
  accessKeyId: config.aws.accessKey,
  secretAccessKey: config.aws.secretAccessKey,
});

export async function uploadFile(filePath: string, key: string): Promise<void> {
  //TODO maybe add tagging for user generated content
  fs.readFile(filePath, async (err, data) => {
    if (err) throw err;
    if (config.env === "dev" ) {
      console.log("env dev, skipping upload")
      return
    }

    const params = {
      Body: data,
      Bucket: IMAGE_BUCKET,
      Key: key,
      ACL: "public-read",
    };

    console.log("s3, begin upload", filePath, key);

    await s3.putObject(params).promise();
    console.log("s3, finished upload", filePath, key);
  });
}
