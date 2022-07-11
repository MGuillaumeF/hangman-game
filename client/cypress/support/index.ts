// coverage support
import "@cypress/code-coverage/support";
import "cypress-movie/commands";
import addContext from "mochawesome/addContext";
import "./commands";

Cypress.on("test:after:run", (test, runnable) => {
  if (test.state === "failed") {
    // `assets/${Cypress.spec.name}/${runnable.parent.title} -- ${test.title} (failed).png`;
    const screenshot = `assets/${
      Cypress.spec.name
    }/${`${runnable.parent.title} -- ${test.title} (failed).png`.replace(
      /^(.*)\s\[@spec:?\s\{.*\}\]\s(--\s.*\s\(failed\)\.png)$/,
      "$1 $2"
    )}`;
    // `${Cypress.config("screenshotsFolder")}/${
    //      Cypress.spec.name
    //    }/${runnable.parent.title} -- ${test.title} (failed).png`;

    addContext({ test }, screenshot);
  }
});
