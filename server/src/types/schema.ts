import { GrowParams } from "./seamCarving";

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
  s3_url: string | null;
  status: Status;
  params: GrowParams; //when I add contentAmp and removeObject, I need to change this to a union type
}

export interface InsertExecutionParams {
  imageName: string;
  routine: string;
  s3Url?: string | null;
  params: GrowParams;
}
