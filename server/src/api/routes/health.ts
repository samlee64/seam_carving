import * as Router from "koa-router";
import { Context } from "koa";

const router = new Router({
  prefix: "/health",
});

router.get("/", async (ctx: Context) => {
  console.log("Health Checked");
  ctx.status = 200;
  ctx.body = { msg: "I'm COVID free!!" };
});

export default router;
