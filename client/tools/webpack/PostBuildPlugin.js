/* eslint-disable @typescript-eslint/no-var-requires */
const Logger = require("./Logger");
const logger = Logger.getInstance();
logger.setLevel("WARN");
logger.setLocation("pioupiou.log");
class PostBuildPlugin {
  apply(compiler) {
    compiler.hooks.done.tap(
      "PostBuildPlugin",
      (
        stats /* stats is passed as an argument when done hook is tapped.  */
      ) => {
        // start reporting generation
        logger.info("APPLY", "PostBuildPlugin", "RUN");
      }
    );
  }
}

module.exports = PostBuildPlugin;
