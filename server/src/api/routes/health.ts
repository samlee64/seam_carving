import * as Router from "koa-router";
import { Context } from "koa";
import { conn } from "../../db";
import { ping } from "../../store/executions";

const router = new Router({
  prefix: "/health",
});

router.get("/", async (ctx: Context) => {
  console.log("Health Checked");
  ctx.status = 200;
  ctx.body = { msg: "I'm COVID free!!" };
});

router.get("/db", async (ctx: Context) => {
  await ping(conn);
  ctx.status = 200;
  ctx.body = { msg: "I'm connected to the db" };
});

export default router;
