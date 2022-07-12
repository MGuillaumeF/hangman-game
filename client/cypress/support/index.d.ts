/// <reference types="cypress" />

declare namespace Cypress {
  interface Chainable {
    /**
     * Clear viewport of test runner
     * @example
     * cy.clearViewport()
     */
    clearViewport(): void;
  }
}

declare module "cypress-sonarqube-reporter/specTitle";
