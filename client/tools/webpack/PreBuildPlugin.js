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
        const apiDocsJson = JSON.parse(apiDocs);
        for (const [endpointName, endpointConfiguration] of Object.entries(
          apiDocsJson.paths
        )) {
          const namesParts = endpointName
            .split(/[\/\-]/g)
            .filter((value) => value !== "");
          console.log("parts", namesParts);
          const name = [
            namesParts.shift(),
            ...namesParts.map((value) =>
              [value[0].toUpperCase(), ...value.slice(1)].join("")
            )
          ].join("");
          parameters[name] = {
            path: `${apiDocsJson.basePath}${endpointName}`
          };
          for (const [methodName, methodConfiguration] of Object.entries(
            endpointConfiguration
          )) {
            parameters[name][methodName] = {};
            for (const parameter of methodConfiguration.parameters) {
              parameters[name][methodName][parameter.name] = {
                name: parameter.name,
                required: parameter.required || false
              };
            }
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
