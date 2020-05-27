import { execFile } from "child_process";

export function execExecutable(filename: string, args: string[]): void {
  execFile(filename, args, (error, stdout, stderr) => {
    if (error) {
      console.error("stderr", stderr);
      console.error("error", error);
      throw error;
    }

    console.log("stdout", stdout);
  });
}
