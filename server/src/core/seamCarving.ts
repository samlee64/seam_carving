import {execExecutable} from '../utils'
import {SeamCarvingParams} from '../types/seamCarving'

export function carveImage(params: SeamCarvingParams): void {
  const filename = params.image;
  const args: string[] = [filename]

  execExecutable(filename, args);
}

export function getResultPath(): string {
  return ""
}
