import { Connection } from "../db";
import { File } from "../types/schema";
import { FILES } from "./constants";

export async function getFiles(
  conn: Connection,
  fileNames: string[]
): Promise<File[]> {
  if (fileNames.length == 0) return [];

  return conn(FILES).whereIn("name", fileNames);
}
