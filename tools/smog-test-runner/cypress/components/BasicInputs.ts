/// <reference types="cypress" />
/**
 * Function to ad value in basic field
 * @param id The id of component
 * @param value value The value to set in component
 * @returns The Chainable Cypress type + check
 */
export function basicInputType(
  id: string,
  value: string
): Cypress.Chainable<JQuery<HTMLElement>> {
  cy.get(`label[for=${id}]`).click();
  return cy.get(`#${id}`).click().type(value).should("have.value", value);
}
