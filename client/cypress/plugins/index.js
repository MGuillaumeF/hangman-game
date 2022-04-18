const registerCodeCoverageTasks = require("@cypress/code-coverage/task");
const cypressMochawesomeReporter = require("cypress-mochawesome-reporter/plugin");
module.exports = (on, config) => {
  registerCodeCoverageTasks(on, config);
  cypressMochawesomeReporter(on);
  // include any other plugin code...

  // https://docs.cypress.io/api/plugins/after-run-api
  on("after:run", (results) => {
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
