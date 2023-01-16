import * as Router from "koa-router";
import { Context } from "koa";
import {
  GrowParams,
  ContentAmplificationParams,
  RemoveObjectParams,
} from "../../types/seamCarving";
import {
  grow,
  contentAmplification,
  removeObject,
} from "../../core/seamCarving";
import { getExecution } from "../../store/executions";
import { conn } from "../../db";
import { listInputImages } from "../../aws/s3";

const router = new Router({
  prefix: "/seam",
});

router.get("/images", async (ctx: Context) => {
  try {
    const ret = await listInputImages();
    ctx.status = 200;
    ctx.body = ret;
  } catch (e) {
    ctx.status = 500;
    ctx.body = { e };
  }
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
    ctx.body = {
      id: execution.id,
      imageName: execution.image_name,
      status: execution.status,
      routine: execution.routine,
      s3Url: execution.s3_url,
    };

    ctx.status = 200;
  } catch (e) {
    ctx.status = 500;
    ctx.body = {};
  }
});

router.post("/grow", async (ctx: Context) => {
  try {
    console.log("grow");
    const params: GrowParams = ctx.request.body;
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

router.post("/content-amplification", async (ctx: Context) => {
  try {
    console.log("content-amplification");
    const params: ContentAmplificationParams = ctx.request.body;
    const executionId = await contentAmplification(conn, params);
    ctx.status = 200;
    ctx.body = { executionId };
  } catch (e) {
    ctx.status = 500;
    ctx.body = e;

    console.error(e);
    return;
  }
});

router.post("/remove-object/markings", async (ctx: Context) => {
  try {
    console.log("remove-object/markings");
    const params: RemoveObjectParams = ctx.request.body;
    const executionId = await removeObject(conn, params);
    ctx.status = 200;
    ctx.body = { executionId };
  } catch (e) {
    ctx.status = 500;
    ctx.body = e;

    console.error(e);
    return;
  }
});

export default router;
