export interface GrowParams {
  addHeight: number;
  addWidth: number;
  imageName: string;
  numSteps: number;
  showIntermediateSteps: boolean;
}

export interface ContentAmplificationParams {
  imageName: string;
  factor: number;
  showIntermediateSteps: boolean;
}

export interface RemoveObjectParams {
  imageName: string;
  showIntermediateSteps: boolean;
  lockRatio: boolean;
  onlyHorizontal: boolean;
  onlyVertical: boolean;
  markings: { destroy: number[]; protect: number[] };
  imageWidth: number;
  imageHeight: number;
}

export enum Routine {
  Grow = "grow",
  ContentAmplification = "contentAmplification",
  RemoveObject = "removeObject",
}

export type RoutineParams =
  | GrowParams
  | ContentAmplificationParams
  | RemoveObjectParams;
