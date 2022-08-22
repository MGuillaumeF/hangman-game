import Logger from "../../../../../client/tools/webpack/Logger.js";

const logger = Logger.getInstance();
logger.setLevel("WARN");
logger.setLocation("webpack-post-build.log");

export default class PostBuildPlugin {
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
