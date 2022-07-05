import { defineConfig } from "cypress";
import webpackConfig from "./webpack.config";
import fs from "fs";
import path from "path";

import registerCodeCoverageTasks from "@cypress/code-coverage/task";
import mergeReports from "cypress-sonarqube-reporter/mergeReports";
export default defineConfig({
  component: {
    devServer(cypressConfig) {
      return devServer({
        framework: "react", // or vue
        bundler: "webpack",
        cypressConfig,
        webpackConfig
      });
    },
    setupNodeEvents(on, config) {
      // component testing node events setup code
      registerCodeCoverageTasks(on, config);
      // include any other plugin code...

      on("after:screenshot", (details) => {
        console.log(details); // print all details to terminal

        const newName = path
          .basename(details.path)
          .replace(
            /^(.*)\s\[@spec:?\s\{.*\}\]\s(--\s.*\s\(failed\)\.png)$/,
            "$1 $2"
          );

        const newPath = path.resolve(path.dirname(details.path), newName);

        return new Promise((resolve, reject) => {
          // fs.rename moves the file to the existing directory 'new/path/to'
          // and renames the image to 'screenshot.png'
          fs.rename(details.path, newPath, (err) => {
            if (err) return reject(err);

            // because we renamed and moved the image, resolve with the new path
            // so it is accurate in the test results
            resolve({ path: newPath });
          });
        });
      });

      // https://docs.cypress.io/api/plugins/after-run-api
      on("after:run", (results) => {
        // /!\ don't forget to return the Promise /!\
        return mergeReports(results, {
          reportsOutputDir: "cypress/reports/sonar",
          mergeOutputDir: "cypress/reports/sonar",
          // see "Merge Plugin Options" section for all available options
          mergeFileName: "sonar-report-all.xml"
        });
      });
    },
    supportFile: "cypress/support/index.js"
  }
});
