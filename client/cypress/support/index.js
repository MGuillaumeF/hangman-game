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

Cypress.on("test:after:run", (test, runnable) => {
  if (test.state === "failed") {
    // `assets/${Cypress.spec.name}/${runnable.parent.title} -- ${test.title} (failed).png`;
    const screenshot = "assets/loading.specs.ts/Errors pages [@spec {nameloading.specs.ts,relativecypressintegrationloading.specs.ts,absolutehomerunnerworkhangman-gamehangman-gameclientcypressintegrationloading.specs.ts,specTypeintegration}] -- Not Found (failed).png"
    // `${Cypress.config("screenshotsFolder")}/${
    //      Cypress.spec.name
    //    }/${runnable.parent.title} -- ${test.title} (failed).png`;
    addContext({ test }, screenshot);
  }
});
