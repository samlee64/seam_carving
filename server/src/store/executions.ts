import { Connection } from "../db";
import { Execution, InsertExecutionParams, Status } from "../types/schema";
import { EXECUTIONS } from "./constants";

export async function getExecution(
  conn: Connection,
  id: string
): Promise<Execution> {
  return conn(EXECUTIONS).select("*").where({ id }).first();
}

export async function insertExecution(
  conn: Connection,
  params: InsertExecutionParams
): Promise<string> {
  const insertParams = {
    image_name: params.imageName,
    routine: params.routine,
    s3_url: params.s3Url,
    params,
  };

  const ret = await conn(EXECUTIONS).insert(insertParams).returning("id");
  console.log(ret);
  //TODO tomorrow do this. Figure out if this is an array or not

  return "";
}

export async function updateStatus(
  conn: Connection,
  id: string,
  status: Status
): Promise<void> {
  conn(EXECUTIONS).update({ status }).where({ id });
}