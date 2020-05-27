import config from "../config";
import { execExecutable } from "../utils";
import { SeamCarvingParams } from "../types/seamCarving";

export function carveImage(params: SeamCarvingParams): void {
  const imageName = params.image;
  const args: string[] = [imageName];

  execExecutable(config.executablePath, args);
}
