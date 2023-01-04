import { defineConfig } from "cypress";
import * as dotenv from "dotenv"; // see https://github.com/motdotla/dotenv#how-do-i-use-dotenv-with-import
const fs = require("fs");
const path = require("path");
dotenv.config();

const HTTP_TEST_PROTOCOL = process.env["HTTP_TEST_PROTOCOL"] || "http";
const HTTP_TEST_PORT =
  process.env["HTTP_TEST_PORT"] ||
  (HTTP_TEST_PROTOCOL === "https" ? "443" : "80");
const HTTP_TEST_DOMAIN_NAME =
  process.env["HTTP_TEST_DOMAIN_NAME"] || "localhost";
const HTTP_TEST_PREFIX_LOCATION =
  process.env["HTTP_TEST_PREFIX_LOCATION"] || "/";

export default defineConfig({
  reporter: "cypress-multi-reporters",
  reporterOptions: {
    reporterEnabled: "cypress-sonarqube-reporter, mochawesome",
    mochawesomeReporterOptions: {
      reportDir: "cypress/reports/tests",
      charts: true,
      html: true,
      json: true,
      reportPageTitle: "Browser Integration Tests Reports",
      embeddedScreenshots: true,
      inlineAssets: true,
      overwrite: true
    },
    cypressSonarqubeReporterReporterOptions: {
      outputDir: "cypress/reports/sonar",
      useAbsoluteSpecPath: true
    }
  },
  screenshotsFolder: "cypress/reports/tests/assets",
  e2e: {
    supportFile: "./cypress/support/e2e.js",
    baseUrl: `${HTTP_TEST_PROTOCOL}://${HTTP_TEST_DOMAIN_NAME}:${HTTP_TEST_PORT}${HTTP_TEST_PREFIX_LOCATION}`,
    setupNodeEvents(on, config) {
      require("@cypress/code-coverage/task")(on, config);
      // tell Cypress to use .babelrc file
      // and instrument the specs files
      // only the extra application files will be instrumented
      // not the spec files themselves
      on("file:preprocessor", require("@cypress/code-coverage/use-babelrc"));

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
          fs.rename(details.path, newPath, (err: any) => {
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
        return require("cypress-sonarqube-reporter/mergeReports")(results, {
          reportsOutputDir: "cypress/reports/sonar",
          mergeOutputDir: "cypress/reports/sonar",
          // see "Merge Plugin Options" section for all available options
          mergeFileName: "sonar-report-all.xml"
        });
      });
      return config;
    }
  },
  env: {
    GITHUB_TOKEN: process.env["GITHUB_TOKEN"]
  }
});
