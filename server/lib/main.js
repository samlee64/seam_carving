"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const utils_1 = require("./utils");
const Koa = require("koa");
const app = new Koa();
app.use((ctx) => {
    try {
        console.log("bues it go");
        utils_1.execExecutable("./scripts/foobar.sh", []);
    }
    catch (e) {
        ctx.status = 500;
        console.error(e);
        return;
    }
    ctx.status = 200;
});
app.listen(3000);
