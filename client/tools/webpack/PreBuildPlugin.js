/* eslint-disable @typescript-eslint/no-var-requires */
const fs = require("fs");
const path = require("path");

const Logger = require("./Logger");
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
        const apiDocs = fs
          .readFileSync(
            path.resolve(__dirname, "../../../server/docs/api_doc.json")
          )
          .toString();
        const parameters = {};
        for (const [endpointName, endpointConfiguration] of Object.entries(
          apiDocs.paths
        )) {
          const namesParts = endpointName.split(/[\/\-]/g);
          const name = [
            namesParts.pop(),
            ...namesParts.map((value) =>
              [value[0].toUpperCas(), ...value.slic(1)].join("")
            )
          ].join("");
          parameters[name] = {};
          for (const [
            methodName,
            methodConfiguration
          ] of endpointConfiguration) {
            parameters[name][methodName] = {};
          }
        }
        console.info(
          "output parameters docs",
          JSON.stringify(parameters, null, 4)
        );
        fs.writeFileSync(
          path.resolve(
            __dirname,
            "../../src/generated/.api_doc_parameters.json"
          ),
          JSON.stringify(parameters, null, 4)
        );
      }
    );
  }
}

module.exports = PreBuildPlugin;
