import * as Router from "koa-router";
import { Context } from "koa";

import { execExecutable } from "../utils";
import { carveImage } from '../core/seamCarving'

const router = new Router({
  prefix: "/seam",
});

router.get("/", async (ctx: Context) => {
  try {
    execExecutable("./core/cmake-build-debug/a6", ["dolphin" ]);
    console.log("done execing");
  } catch (e) {
    ctx.status = 500;
    console.error(e);
    return;
  }

  ctx.status = 200;
  ctx.body = {imageName: "sam"}
});

router.post("/carve", async (ctx: Context) => {
  const params = ctx.body;
  try {
    carveImage(params)

  } catch(e) {
    ctx.status = 500;
    ctx.body = e

    console.error(e)
    return;
  }
})

export default router;
