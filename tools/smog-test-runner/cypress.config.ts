import { defineConfig } from "cypress";
import * as dotenv from "dotenv"; // see https://github.com/motdotla/dotenv#how-do-i-use-dotenv-with-import
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
    // "reporterEnabled": "cypress-sonarqube-reporter, mochawesome",
    reporterEnabled: "mochawesome",
    mochawesomeReporterOptions: {
      reportDir: "cypress/reports/tests",
      charts: true,
      html: true,
      json: true,
      reportPageTitle: "Browser Integration Tests Reports",
      embeddedScreenshots: true,
      inlineAssets: true,
      overwrite: true
    }
    /* "cypressSonarqubeReporterReporterOptions": {
      "outputDir": "cypress/reports/sonar",
      "useAbsoluteSpecPath": true
    }*/
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
      return config;
    }
  },
  env: {
    GITHUB_TOKEN: process.env["GITHUB_TOKEN"]
  }
});
