import * as Router from "koa-router";
import { Context } from "koa";
import { GrowParams } from "../types/seamCarving";
import { grow } from "../core/seamCarving";

const router = new Router({
  prefix: "/seam",
});

router.post("/grow", async (ctx: Context) => {
  const params: GrowParams = ctx.request.body;
  console.log(params);

  try {
    grow(params);
    ctx.status = 200;
    ctx.body = "Execution has been sent";
  } catch (e) {
    ctx.status = 500;
    ctx.body = e;

    console.error(e);
    return;
  }

  console.log("response");
});

export default router;
