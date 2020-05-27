import * as Router from "koa-router";
import * as send from "koa-send";
import { Context } from "koa";
import config from "../config";

const router = new Router({
  prefix: "/files",
});

router.get("/", async (ctx: Context) => {
  console.log("this is the path", ctx.path);
  console.log("this is the path", __dirname);

  console.log("this is the datadir", config.dataDir);

  await send(ctx, "output/surf-vertical.png", { root: config.dataDir });
});

export default router;
