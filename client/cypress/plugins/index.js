const fs = require("fs");
const path = require("path");

const registerCodeCoverageTasks = require("@cypress/code-coverage/task");

module.exports = (on, config) => {
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
    return require("cypress-sonarqube-reporter/mergeReports")(results, {
      reportsOutputDir: "cypress/reports/sonar",
      mergeOutputDir: "cypress/reports/sonar",
      // see "Merge Plugin Options" section for all available options
      mergeFileName: "sonar-report-all.xml"
    });
  });

  // It's IMPORTANT to return the config object
  // with any changed environment variables
  return config;
};
