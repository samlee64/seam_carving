import * as path from "path";
import config from "../config";

export interface OutputPath {
  fileName: string;
  path: string;
}

export function outputPaths(imageName: string): OutputPath[] {
  //Returns the location of the newly written png and gif files
  return ["mask.gif", "energy-map.gif", "mid.gif", "output.png"].map(
    (fileName: string) => {
      return {
        fileName,
        path: path.join(config.dataDir, "output", imageName, fileName),
      };
    }
  );
}

export function s3Path(imageName: string, fileName: string): string {
  return path.join("output", imageName, fileName);
}
