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
  onlyHorizontal : boolean;
  onlyVertical : boolean;
  protectedRegions: number[][][];
  destroyRegions: number[][][];
}

export enum Routine {
  Grow = "grow",
  ContentAmplification = "contentAmplification",
}

export type RoutineParams = GrowParams | ContentAmplificationParams;
