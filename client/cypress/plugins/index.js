const codeCoverageTask = require('@cypress/code-coverage/task');
module.exports = (on, config) => {
  codeCoverageTask(on, config)
  // include any other plugin code...

  // It's IMPORTANT to return the config object
  // with any changed environment variables
  return config
}
