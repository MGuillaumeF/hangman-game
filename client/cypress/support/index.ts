/// <reference types="cypress" />

// coverage support
import "@cypress/code-coverage/support";
import "cypress-movie/commands";
import addContext from "mochawesome/addContext";

const COMMAND_DELAY = 300;

for (const command of [
  "visit",
  "click",
  "trigger",
  "type",
  "clear",
  "reload",
  "contains",
  "submit"
]) {
  Cypress.Commands.overwrite(command, (originalFn, ...args) => {
    const origVal = originalFn(...args);

    return new Promise((resolve) => {
      setTimeout(() => {
        resolve(origVal);
      }, COMMAND_DELAY);
    });
  });
}

Cypress.Commands.add("clearViewport", () => {
  const runnerContainer =
    window.parent.document.getElementsByClassName("iframes-container")[0];
  if (runnerContainer) {
    runnerContainer.setAttribute(
      "style",
      "left: 0; top: 0; width: 100%; height: 100%;"
    );
  }

  const sizeContainer =
    window.parent.document.getElementsByClassName("size-container")[0];
  if (sizeContainer) {
    sizeContainer.setAttribute("style", "");
  }

  const sidebar =
    window.parent.document.getElementsByClassName("reporter-wrap")[0];
  if (sidebar) {
    sidebar.setAttribute("style", "opacity: 0");
  }

  const header = window.parent.document.querySelector(
    ".runner.container header"
  );
  if (header) {
    header.setAttribute("style", "opacity: 0");
  }
});

Cypress.on("test:after:run", (test, runnable) => {
  if (test["state"] === "failed" && runnable["parent"]) {
    // `assets/${Cypress.spec.name}/${runnable.parent.title} -- ${test.title} (failed).png`;
    const screenshot = `assets/${
      Cypress.spec.name
    }/${`${runnable["parent"]["title"]} -- ${test["title"]} (failed).png`.replace(
      /^(.*)\s\[@spec:?\s\{.*\}\]\s(--\s.*\s\(failed\)\.png)$/,
      "$1 $2"
    )}`;
    // `${Cypress.config("screenshotsFolder")}/${
    //      Cypress.spec.name
    //    }/${runnable.parent.title} -- ${test.title} (failed).png`;
    // @ts-ignore
    addContext({ test }, screenshot);
  }
});
