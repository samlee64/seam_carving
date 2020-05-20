
import * as Router from "koa-router";
import * as send from "koa-send";
import { Context } from "koa";


const router = new Router({
  prefix: "/files",
});

router.get("/", async (ctx: Context) => {
  console.log("this is the path", ctx.path)
  console.log("this is the path", __dirname)

  await send(ctx, ctx.path, {root: __dirname})
});


export default router;
