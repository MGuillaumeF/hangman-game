// ***********************************************************
// This example support/index.js is processed and
// loaded automatically before your test files.
//
// This is a great place to put global configuration and
// behavior that modifies Cypress.
//
// You can change the location of this file or turn off
// automatically serving support files with the
// 'supportFile' configuration option.
//
// You can read more here:
// https://on.cypress.io/configuration
// ***********************************************************

// Import commands.js using ES2015 syntax:
import "./commands";

// Alternatively you can use CommonJS syntax:
// require('./commands')

// coverage support
import "@cypress/code-coverage/support";

import "cypress-movie/commands";

import addContext from "mochawesome/addContext";

import fs from "fs";

Cypress.on("test:after:run", (test, runnable) => {
  if (test.state === "failed") {
    // `assets/${Cypress.spec.name}/${runnable.parent.title} -- ${test.title} (failed).png`;
    const screenshot = `${Cypress.config("screenshotsFolder")}/${
      Cypress.spec.name
    }/Errors pages -- Not Found (failed).png`;
    // `${Cypress.config("screenshotsFolder")}/${
    //      Cypress.spec.name
    //    }/${runnable.parent.title} -- ${test.title} (failed).png`;
    fs.writeFileSync(
      Cypress.config("screenshotsFolder") + "/assets/dump.txt",
      `add screenshot in report failed case, ${screenshot}, file exist ? ${fs.existsSync(
        screenshot
      )}`
    );
    addContext({ test }, screenshot);
  }
});
