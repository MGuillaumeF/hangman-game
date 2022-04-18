const registerCodeCoverageTasks = require("@cypress/code-coverage/task");
const cypressMochawesomeReporter = require('cypress-mochawesome-reporter/plugin');
module.exports = (on, config) => {
  registerCodeCoverageTasks(on, config);
  cypressMochawesomeReporter(on);
  // include any other plugin code...

  // It's IMPORTANT to return the config object
  // with any changed environment variables
  return config;
};
