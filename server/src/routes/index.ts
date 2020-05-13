import * as Router from "koa-router";

import health from "./health";
import seamCarving from "./seamCarving";

const router = new Router();

router.use(health.routes());
router.use(seamCarving.routes());

export default router;
