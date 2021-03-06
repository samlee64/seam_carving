import { RoutineParams } from "./seamCarving";

export enum Status {
  Executing = "executing",
  Uploading = "uploading",
  Done = "done",
  Error = "error",
}

export interface Execution {
  id: string;
  image_name: string;
  routine: string;
  s3_url: string[] | null;
  status: Status;
  params: RoutineParams;
  output_files: string[] | null;
}

//TODO do something fancy binding the type of routine to the type of a params
export interface InsertExecutionParams {
  imageName: string;
  routine: string;
  s3Url?: string[] | null;
  outputFiles?: string[] | null;
  params: RoutineParams;
}

export interface File {
  name: string;
  metadata: { [key: string]: string };
  comment: string;
}
