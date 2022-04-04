/* eslint-disable @typescript-eslint/no-var-requires */
const Logger = require("./Logger");
const fs = require("fs");
const path = require("path");
const logger = Logger.getInstance();
logger.setLevel("WARN");
logger.setLocation("pioupiou.log");
class PreBuildPlugin {
  apply(compiler) {
    compiler.hooks.beforeCompile.tap(
      "PreBuildPlugin",
      (
        stats /* stats is passed as an argument when done hook is tapped.  */
      ) => {
        // start reporting generation
        logger.info("APPLY", "PreBuildPlugin", "RUN");
        console.log(...Object.keys(stats));
        if (!fs.existsSync(path.resolve(__dirname, "../../src/generated"))) {
          fs.mkdirSync(path.resolve(__dirname, "../../src/generated"));
        }
        fs.copyFileSync(
          path.resolve(__dirname, "../../../server/docs/api_doc.json"),
          path.resolve(__dirname, "../../src/generated/.api_doc.json")
        );
      }
    );
  }
}

module.exports = PreBuildPlugin;
