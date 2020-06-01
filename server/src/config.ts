export interface Config {
  port: number;
  dataDir: string;
  executablePath: string;
  aws: {
    accessKey: string;
    secretAccessKey: string;
  };
  db: {
    host: string;
    user: string;
    password: string;
    database: string;
    port: number;
  };
}

//tslint:disable-next-line
const staticConfig = require("../config.json");

const config: Config = {
  ...staticConfig,
  dataDir: __dirname + "/../data",
  executablePath: __dirname + "/../core/cmake-build-debug/seam_carving",
};

export default config;
