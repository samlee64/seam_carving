import { execExecutable } from "./utils";
import * as Koa from 'koa'

const app = new Koa();

app.use((ctx: Koa.Context) => {
  try {
    execExecutable("./scripts/foobar.sh", []);
  } catch (e) {
    ctx.status = 500;
    console.error(e);
    return;
  }

  ctx.status = 200;
});

app.listen(3000);
