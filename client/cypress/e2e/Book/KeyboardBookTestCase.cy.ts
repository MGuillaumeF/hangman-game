// type definitions for Cypress object "cy"
/// <reference types="cypress" />

// type definitions for custom commands like "createDefaultTodos"
/// <reference types="../../src/support" />

// check this file using TypeScript if available
// @ts-check

import specTitle from "cypress-sonarqube-reporter/specTitle";

describe(specTitle("Keyboard test"), () => {
  beforeEach("load root book page", () => {
    cy.visit("/book/");
  });

  it("QWERTY test", () => {
    // load keyboard page
    cy.get("#PAGES_KEYBOARD").click();
    // check the first letter of keyboard is a "A" because by default the keyboard is azerty
    cy.get("#keyboard-component").should("be.visible").eq(0).contains("A");

    // check A key is not disabled and press key
    cy.get("#keyboard-component-keyboard-A")
      .contains("A")
      .should("not.be.disabled")
      .click();

    // check A key is added to used keys
    cy.get("#keyboard-book-used-letters").contains("A");

    // check A key is disabled because already used
    cy.get("#keyboard-component-keyboard-A")
      .contains("A")
      .should("be.disabled");

    // change keyboard to qwerty
    cy.get("#disposition-updater").click();

    // check first key is a Q and is not disabled and press key
    cy.get("#keyboard-component")
      .should("be.visible")
      .eq(0)
      .contains("Q")
      .click();

    // check Qs key is added to used keys
    cy.get("#keyboard-book-used-letters").contains("Q");
  });
});
