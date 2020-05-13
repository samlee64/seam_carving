import * as Koa from "koa";
import * as bodyParser from "koa-bodyparser";
import * as json from "koa-json";
import router from "./routes";

const app = new Koa();

//middleware
app.use(bodyParser());
app.use(json());

//routes
app.use(router.routes());

app.listen(3000, () => {
  console.log("Running...");
});
