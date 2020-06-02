import * as Router from "koa-router";
import { Context } from "koa";
import { GrowParams } from "../../types/seamCarving";
import { grow } from "../../core/seamCarving";
import { getExecution } from "../../store/executions";
import { conn } from "../../db";

const router = new Router({
  prefix: "/seam",
});

router.get("/status", async (ctx: Context) => {
  const executionId = ctx.query.executionId;

  if (!executionId) {
    ctx.status = 400;
    ctx.body = "Include query string param executionId";
    return;
  }

  try {
    const execution = await getExecution(conn, executionId);
    ctx.status = 200;
    ctx.body = {id: execution.id, imageName: execution.image_name, status: execution.status};
  } catch (e) {}
});

router.post("/grow", async (ctx: Context) => {
  const params: GrowParams = ctx.request.body;
  try {
    const executionId = await grow(conn, params);
    ctx.status = 200;
    ctx.body = { ...params, executionId };
  } catch (e) {
    ctx.status = 500;
    ctx.body = e;

    console.error(e);
    return;
  }
});

export default router;
