// load type definitions that come with Cypress module
/// <reference types="cypress" />
import "@cypress/code-coverage/support";
import "cypress-movie/commands";
import { Context } from "mocha";
import addContext from "mochawesome/addContext";
import "./commands";

Cypress.on("test:after:run", (test, runnable) => {
  if (test["state"] === "failed" && runnable?.parent) {
    // `assets/${Cypress.spec.name}/${runnable.parent.title} -- ${test.title} (failed).png`;
    const screenshot = `assets/${
      Cypress.spec.name
    }/${`${runnable.parent.title} -- ${test["title"]} (failed).png`.replace(
      /^(.*)\s\[@spec:?\s\{.*\}\]\s(--\s.*\s\(failed\)\.png)$/,
      "$1 $2"
    )}`;
    // `${Cypress.config("screenshotsFolder")}/${
    //      Cypress.spec.name
    //    }/${runnable.parent.title} -- ${test.title} (failed).png`;

    addContext({ test } as Context, screenshot);
  }
});
