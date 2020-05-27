import * as Router from "koa-router";

import health from "./health";
import seamCarving from "./seamCarving";
import files from "./files";

const router = new Router();

router.use(health.routes());
router.use(seamCarving.routes());
router.use(files.routes());

export default router;
