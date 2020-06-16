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
    params: params.params,
  };

  const ret = await conn(EXECUTIONS).insert(insertParams).returning("id");
  return ret[0];
}

export async function updateStatus(
  conn: Connection,
  id: string,
  status: Status
): Promise<void> {
  await conn(EXECUTIONS).update({ status }).where({ id });
}

export async function updateS3Keys(
  conn: Connection,
  id: string,
  s3Keys: string[]
): Promise<void> {
  await conn(EXECUTIONS).update({ s3_url: s3Keys }).where({ id });
}
