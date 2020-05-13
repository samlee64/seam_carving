import * as Router from "koa-router";
import { Context } from "koa";

import { execExecutable } from "../utils";

const router = new Router({
  prefix: "seam",
});

router.get("/", async (ctx: Context) => {
  try {
    execExecutable("./core/cmake-build-debug/a6", [
      "first argument",
      "second argument",
    ]);
    console.log("done execing");
  } catch (e) {
    ctx.status = 500;
    console.error(e);
    return;
  }

  ctx.status = 200;
});

export default router;
