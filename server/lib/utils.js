"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.execExecutable = void 0;
const child_process_1 = require("child_process");
function execExecutable(filename, args) {
    child_process_1.execFile(filename, args, (error, stdout, stderr) => {
        if (error) {
            console.error("stderr", stderr);
            throw error;
        }
        console.log("stdout", stdout);
    });
}
exports.execExecutable = execExecutable;
