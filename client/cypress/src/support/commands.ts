/**
 * Custom command to remove cypress left test list section
 * @example cy.clearViewport()
 */
Cypress.Commands.add(
  "clearViewport",
  (): Cypress.Chainable<JQuery<HTMLBodyElement>> => {
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

    return cy.get("body");
  }
);

const AUTHOR = "MGuillaumeF";
const PROJECT = "Hangman Game";
const VERSION = "v0.1.0";

/**
 * Function to add empty black page in test
 * @param id The id
 * @param elements elements to add in pag
 * @returns Function to add blac page in test
 */
Cypress.Commands.add(
  "addPage",
  (id: string, ...elements: any[]): Cypress.Chainable<Document> => {
    return cy.document().then(($document) => {
      cy.get("body").then(($div) => {
        function add() {
          const page = $document.createElement("div");
          page.id = `cypress-application-${id}`;
          page.style.backgroundColor = "#333";
          page.style.color = "#FFF";
          page.style.height = "100vh";
          page.style.width = "100vw";
          page.style.position = "fixed";
          page.style.display = "flex";
          page.style.alignItems = "center";
          page.style.justifyContent = "center";
          page.style.flexDirection = "column";
          page.style.zIndex = "3000000";

          elements.forEach((element) => page.append(element));
          return page;
        }
        $div.empty();
        $div.append(add);
      });
    });
  }
);

/**
 * Function to add presentation page in test
 * @param title The title of tests
 * @param project The project of tests
 * @param version Theverion of project
 * @param author The author of project
 * @returns The added page
 */
Cypress.Commands.add(
  "addPresentation",
  (
    title: string,
    project: string = PROJECT,
    version: string = VERSION,
    author: string = AUTHOR
  ): Cypress.Chainable<Document> => {
    return cy.document().then(($document) => {
      const mainTitle = $document.createElement("h1");
      mainTitle.textContent = project;

      const subTitle = $document.createElement("h2");
      subTitle.textContent = title;

      const pVersion = $document.createElement("p");
      pVersion.textContent = version;

      const date = $document.createElement("span");
      // date.textContent = new Date().toISOString().split("T")[0];
      date.style.position = "fixed";
      date.style.bottom = "0.5em";
      date.style.right = "0.5em";

      const sAuthor = $document.createElement("span");
      sAuthor.textContent = author;
      sAuthor.style.position = "fixed";
      sAuthor.style.top = "0.5em";
      sAuthor.style.left = "0.5em";

      return cy.addPage("pres", mainTitle, subTitle, pVersion, date, sAuthor);
    });
  }
);

/**
 * Function to add End page in test displaying
 * @returns The added page
 */
Cypress.Commands.add("addEnd", () => {
  return cy.document().then(($document) => {
    const title = $document.createElement("h1");
    title.textContent = "FIN";
    return cy.addPage("end", title);
  });
});

/**
 * Function to ad value in basic field
 * @param id The id of component
 * @param value value The value to set in component
 * @returns The Chainable Cypress type + check
 */

Cypress.Commands.add(
  "basicInputType",
  (id: string, value: string): Cypress.Chainable<JQuery<HTMLElement>> => {
    cy.get(`label[for=${id}]`).click();
    return cy.get(`#${id}`).click().type(value).should("have.value", value);
  }
);

const COMMAND_DELAY = 300;

for (const command of [
  "visit",
  "click",
  "trigger",
  "type",
  "clear",
  "reload",
  "submit"
]) {
  Cypress.Commands.overwrite(
    command as keyof Cypress.Chainable<any>,
    (originalFn, ...args) => {
      const origVal = originalFn(...args);

      return new Promise((resolve) => {
        setTimeout(() => {
          resolve(origVal);
        }, COMMAND_DELAY);
      });
    }
  );
}
