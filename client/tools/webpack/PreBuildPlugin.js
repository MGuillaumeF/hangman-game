/* eslint-disable @typescript-eslint/no-var-requires */
const fs = require("fs");
const path = require("path");

const Logger = require("./Logger");
const logger = Logger.getInstance();
logger.setLevel("WARN");
logger.setLocation("pre-build-plugin.log");

class PreBuildPlugin {
  apply(compiler) {
    compiler.hooks.environment.tap("PreBuildPlugin", () => {
      this.initGeneratedDirectory();
      this.copyApiDoc();
      this.generateApiParameters();
    });
    compiler.hooks.beforeCompile.tap(
      "PreBuildPlugin",
      (
        stats /* stats is passed as an argument when done hook is tapped.  */
      ) => {
        console.log("BEFORE COMPILE STATS DATAS", ...Object.keys(stats));
        this.initGeneratedDirectory();
        this.copyApiDoc();
        this.generateApiParameters();
      }
    );
  }
  initGeneratedDirectory() {
    if (!fs.existsSync(path.resolve(__dirname, "../../src/generated"))) {
      fs.mkdirSync(path.resolve(__dirname, "../../src/generated"));
    }
  }
  copyApiDoc() {
    let mustBeCopied = false;
    if (
      !fs.existsSync(
        path.resolve(__dirname, "../../src/generated/.api_doc.json")
      )
    ) {
      mustBeCopied = true;
    } else {
      const externalInputDoc = fs.readFileSync(
        path.resolve(__dirname, "../../../server/docs/api_doc.json")
      );
      const currentInputDoc = fs.readFileSync(
        path.resolve(__dirname, "../../src/generated/.api_doc.json")
      );
      if (!externalInputDoc.equals(currentInputDoc)) {
        mustBeCopied = true;
      }
    }
    if (mustBeCopied) {
      console.log("COPY DOC FILE");
      fs.copyFileSync(
        path.resolve(__dirname, "../../../server/docs/api_doc.json"),
        path.resolve(__dirname, "../../src/generated/.api_doc.json")
      );
    }
  }
  generateApiParameters() {
    // start reporting generation
    logger.info("GENERATION", "PreBuildPlugin", "RUN");

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
          `${value[0].toUpperCase()}${value.slice(1)}`
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
    let mustBeWrited = false;
    const dataToWrite = JSON.stringify(parameters, null, 4);
    if (
      !fs.existsSync(
        path.resolve(
          path.resolve(
            __dirname,
            "../../src/generated/.api_doc_parameters.json"
          )
        )
      )
    ) {
      mustBeWrited = true;
    } else {
      const currentInputDoc = fs.readFileSync(
        path.resolve(__dirname, "../../src/generated/.api_doc_parameters.json")
      );
      if (currentInputDoc.toString() !== dataToWrite) {
        mustBeWrited = true;
      }
    }
    if (mustBeWrited) {
      console.info(
        "output parameters docs",
        JSON.stringify(parameters, null, 4)
      );
      fs.writeFileSync(
        path.resolve(__dirname, "../../src/generated/.api_doc_parameters.json"),
        dataToWrite
      );
    }
  }
}

module.exports = PreBuildPlugin;
