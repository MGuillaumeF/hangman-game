const registerCodeCoverageTasks = require("@cypress/code-coverage/task");
const {
  beforeRunHook,
  afterRunHook
} = require("cypress-mochawesome-reporter/lib");

module.exports = (on, config) => {
  registerCodeCoverageTasks(on, config);
  // include any other plugin code...

  on("before:run", async (details) => {
    await beforeRunHook(details);
  });

  // https://docs.cypress.io/api/plugins/after-run-api
  on("after:run", async (results) => {
    await afterRunHook();
    // /!\ don't forget to return the Promise /!\
    return require("cypress-sonarqube-reporter/mergeReports")(results, {
      // see "Merge Plugin Options" section for all available options
      mergeFileName: "sonar-report-all.xml"
    });
  });

  // It's IMPORTANT to return the config object
  // with any changed environment variables
  return config;
};
