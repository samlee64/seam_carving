import * as fs from "fs";
import * as path from "path";
import { PNG } from "pngjs";
import config from "../config";

interface CreateMaskParams {
  width: number;
  height: number;
  data: number[]; //binary array 1 indicating mask placement
}

export async function createMask(
  params: CreateMaskParams,
  imageName: string,
  fileName: string
): Promise<string> {
  const png = new PNG({
    width: params.width,
    height: params.height,
    colorType: 0,
    inputColorType: 0,
  });

  for (let idx = 0; idx < params.data.length; idx++) {
    if (params.data[idx] !== 0) png.data[idx] = 255;
  }

  const dirPath = path.join(config.dataDir, "output/mask", imageName);
  const fullPath = path.join(dirPath, fileName);
  await fs.promises.mkdir(dirPath, { recursive: true });

  png.pack().pipe(fs.createWriteStream(fullPath));

  return fullPath;
}
