import * as Knex from "knex";
import config from "./config";

export type Connection = Knex;

export const conn = Knex({
  client: "pg",
  connection: {
    user: config.db.user,
    password: config.db.password,
    database: config.db.database,
    port: config.db.port,
    host: config.db.host,
  },
});
