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
    ctx.body = {
      id: execution.id,
      imageName: execution.image_name,
      status: execution.status,
      routine: execution.routine,
      s3Url: execution.s3_url,
    };
  } catch (e) {
    ctx.status = 500;
    ctx.body = {};
  }
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

router.post("/content-amplification", async (ctx: Context) => {
  const params: ContentAmplificationParams = ctx.request.body;
  try {
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

router.post("/remove-object", async (ctx: Context) => {
  const params: RemoveObjectParams = ctx.request.body;
  try {
    console.log("removeobject params", JSON.stringify(params));
    //    const executionId = await contentAmplification(conn, params);
    ctx.status = 200;
    //    ctx.body = { executionId };
  } catch (e) {
    ctx.status = 500;
    ctx.body = e;

    console.error(e);
    return;
  }
});

router.post("/remove-object/markings", async (ctx: Context) => {
  try {
    /*
    const items = JSON.parse(ctx.request.body.markings.protect);
    console.log(JSON.stringify(items));

    fs.writeFile('dolphin-triangle.json', JSON.stringify(items), (e) => {
      console.error(e);
    });
    */

    /*
    //    const png = new PNG.PNG({ width: 239, height: 200, colorType: 0, inputColorType: 0 });

    //    console.log('got somnething', items, items.length);
    //  for (let idx = 0; idx < items.length; idx++) {
    //    if (items[idx] != 0) {
    //      png.data[idx] = 255;
    //    }
    //  }
    //  png.pack().pipe(fs.createWriteStream('sam.png'));
    //    const executionId = await contentAmplification(conn, params);
   */
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
