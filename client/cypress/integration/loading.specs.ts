/* eslint-disable promise/no-nesting */
/* eslint-disable promise/catch-or-return */
function addPage(id: string, ...elements: any[]) {
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
        page.style.zIndex = 3000000;

        elements.forEach((element) => page.append(element));
        return page;
      }
      $div.empty();
      $div.append(add);
    });
  });
}

function addPresentation() {
  return cy.document().then(($document) => {
    const title = $document.createElement("h1");
    title.textContent = "Hangman Game";

    const subTitle = $document.createElement("h2");
    subTitle.textContent = "Démonstration";

    const version = $document.createElement("p");
    version.textContent = "v0.1.0";

    const date = $document.createElement("span");
    date.textContent = new Date().toISOString().split("T")[0];
    date.style.position = "fixed";
    date.style.bottom = "0.5em";
    date.style.right = "0.5em";

    const author = $document.createElement("span");
    author.textContent = "MGuillaumeF";
    author.style.position = "fixed";
    author.style.top = "0.5em";
    author.style.left = "0.5em";

    return addPage("pres", title, subTitle, version, date, author);
  });
}

function addEnd() {
  return cy.document().then(($document) => {
    const title = $document.createElement("h1");
    title.textContent = "FIN";
    return addPage("end", title);
  });
}

describe("Load presentation page", () => {
  it("load first page", () => {
    cy.clearViewport();
    cy.visit("/");

    addPresentation();

    cy.wait(3000);
    cy.visit("/");
    // all parameters are optional
    cy.toast("Change language to French", {
      blocking: true
    });
    cy.get("#lang-fr").click();
    cy.contains("Paramètre");

    // all parameters are optional
    cy.toast("Change language to English", {
      blocking: true
    });
    cy.get("#lang-en").click();
    cy.contains("Settings");

    cy.get("#PAGES_SETTINGS_TITLE").click();

    // all parameters are optional
    cy.toast("Display menu", {
      blocking: true
    });

    cy.visit("/badPage");
    cy.contains("Error404");

    addEnd();
  });
});
