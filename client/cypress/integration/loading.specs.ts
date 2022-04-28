/* eslint-disable promise/no-nesting */
/* eslint-disable promise/catch-or-return */

/// <reference types="cypress" />

import { expect } from "chai";
import specTitle from "cypress-sonarqube-reporter/specTitle";
import { CyHttpMessages } from "cypress/types/net-stubbing";
import { basicInputType } from "./components/BasicInputs";
import { addEnd, addPresentation } from "./presentation";

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
