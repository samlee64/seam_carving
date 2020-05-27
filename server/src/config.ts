export interface Config {
  port: number;
  dataDir: string;
  executablePath: string;
  aws: {
    accessKey: string;
    secretAccessKey: string;
  };
}

//tslint:disable-next-line
const staticConfig = require("../config.json");

const config: Config = {
  ...staticConfig,
  dataDir: __dirname + "/../data",
  executablePath: __dirname + "/../core/cmake-build-debug/a6",
};

export default config;
