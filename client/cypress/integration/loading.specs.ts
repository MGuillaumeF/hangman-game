/* eslint-disable promise/no-nesting */
/* eslint-disable promise/catch-or-return */

/// <reference types="cypress" />

import { expect } from "chai";
import specTitle from "cypress-sonarqube-reporter/specTitle";
import { CyHttpMessages } from "cypress/types/net-stubbing";

const AUTHOR = "MGuillaumeF";
const PROJECT = "Hangman Game";
const VERSION = "v0.1.0";

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
        page.style.zIndex = "3000000";

        elements.forEach((element) => page.append(element));
        return page;
      }
      $div.empty();
      $div.append(add);
    });
  });
}

function addPresentation(
  title: string,
  project: string = PROJECT,
  version: string = VERSION,
  author: string = AUTHOR
) {
  return cy.document().then(($document) => {
    const mainTitle = $document.createElement("h1");
    mainTitle.textContent = project;

    const subTitle = $document.createElement("h2");
    subTitle.textContent = title;

    const pVersion = $document.createElement("p");
    pVersion.textContent = version;

    const date = $document.createElement("span");
    date.textContent = new Date().toISOString().split("T")[0];
    date.style.position = "fixed";
    date.style.bottom = "0.5em";
    date.style.right = "0.5em";

    const sAuthor = $document.createElement("span");
    sAuthor.textContent = author;
    sAuthor.style.position = "fixed";
    sAuthor.style.top = "0.5em";
    sAuthor.style.left = "0.5em";

    return addPage("pres", mainTitle, subTitle, pVersion, date, sAuthor);
  });
}

function addEnd() {
  return cy.document().then(($document) => {
    const title = $document.createElement("h1");
    title.textContent = "FIN";
    return addPage("end", title);
  });
}

function basicInputType(id: string, value: string) {
  cy.get(`label[for=${id}]`).click();
  return cy.get(`#${id}]`).click().type(value).should("have.value", value);
}

describe(specTitle("Offlines Access"), () => {
  it("Settings", () => {
    cy.clearViewport();
    cy.visit("/");

    addPresentation("Settings");

    cy.wait(2000);
    cy.visit("/");
    // all parameters are optional
    cy.toast("Change language to French", {
      blocking: true
    });
    cy.get("#lang-fr").click();
    cy.contains("Connexion");

    // all parameters are optional
    cy.toast("Change language to English", {
      blocking: true
    });
    cy.get("#lang-en").click();
    cy.contains("Sign In");

    cy.get("#PAGES_SETTINGS_TITLE").click();

    // all parameters are optional
    cy.toast("Display menu", {
      blocking: true
    });

    addEnd();
  });
});

describe(specTitle("Subscription"), () => {
  it("With Success", () => {
    cy.clearViewport();
    cy.visit("/");

    addPresentation("Subscription - with success");

    cy.wait(2000);
    cy.visit("/");

    cy.intercept(
      "POST",
      "/api/v1/user/sign-up",
      (req: CyHttpMessages.IncomingHttpRequest) => {
        expect(req.body?.email).to.equal("mguillaumef@draft.com");
        expect(req.body?.login).to.equal("mguillaumef");
        expect(req.body?.password).to.equal("DROWssap987");
        expect(req.body?.confirm).to.equal("DROWssap987");
      }
    ).as("subscribe");

    cy.get("#PAGES_SIGN_UP_LINK").click();

    basicInputType("FORMS_SIGN_UP_FIELDS_EMAIL", "mguillaumef@draft.com");

    basicInputType("FORMS_SIGN_UP_FIELDS_IDENTIFIER", "mguillaumef");

    basicInputType("FORMS_SIGN_UP_FIELDS_PASSWORD", "DROWssap987");

    basicInputType("FORMS_SIGN_UP_FIELDS_CONFIRM", "DROWssap987").type(
      "{enter}"
    );

    cy.wait("@subscribe");
    addEnd();
  });
});

describe(specTitle("Login"), () => {
  it("With Success", () => {
    cy.clearViewport();
    cy.visit("/");

    addPresentation("Login - with success");

    cy.wait(2000);
    cy.visit("/");

    cy.intercept(
      "POST",
      "/api/v1/user/sign-in",
      (req: CyHttpMessages.IncomingHttpRequest) => {
        expect(req.body?.login).to.equal("mguillaumef");
        expect(req.body?.password).to.equal("DROWssap987");
      }
    ).as("login");

    cy.get("#PAGES_SIGN_IN_LINK").click();

    basicInputType("FORMS_SIGN_IN_FIELDS_IDENTIFIER", "mguillaumef");

    basicInputType("FORMS_SIGN_IN_FIELDS_PASSWORD", "DROWssap987").type(
      "{enter}"
    );

    cy.wait("@login");
    addEnd();
  });
});

describe(specTitle("Errors pages"), () => {
  it("Not Found", () => {
    cy.clearViewport();
    cy.visit("/");

    addPresentation("Not Found");

    cy.wait(2000);
    cy.visit("/badPage");
    cy.contains("Error404");

    addEnd();
  });
});
