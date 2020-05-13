import { execFile } from "child_process";

export type SeamCarvingParam = {
  protectedObject: number[];
  destroyObject: number[];
  image: string; //maybe define this in a config so it can be shared with core
};

export function execExecutable(filename: string, args: string[]): void {
  execFile(filename, args, (error, stdout, stderr) => {
    if (error) {
      console.error("stderr", stderr);
      throw error;
    }

    console.log("stdout", stdout);
  });
}
