import * as fs from "fs";
import * as path from "path";
import { PNG } from "pngjs";
import config from "../config";

interface CreateMaskParams {
  width: number;
  height: number;
  data: number[];
}

export function createMask(
  params: CreateMaskParams,
  imageName: string,
  filename: string
): string {
  //do a validation on the data;
  //data should be equal to width * height;
  const data = params.data;

  const png = new PNG({
    width: params.width,
    height: params.height,
    colorType: 0,
    inputColorType: 0,
  });
  for (let idx = 0; idx < data.length; idx++) {
    if (data[idx] != 0) png.data[idx] = 255;
  }

  const dirPath = path.join(config.dataDir, "output/mask", imageName);
  const fullPath = path.join(dirPath, filename);

  fs.mkdirSync(dirPath, { recursive: true });

  png.pack().pipe(fs.createWriteStream(fullPath));

  return fullPath;
}
