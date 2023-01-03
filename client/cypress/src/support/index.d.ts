// load type definitions that come with Cypress module
/// <reference types="cypress" />
import "@cypress/code-coverage/support.ts";

declare global {
  namespace Cypress {
    interface Chainable {
      /**
       * Function to add End page in test displaying
       * @returns The added page
       */
      addEnd(): Cypress.Chainable<Document>;
      /**
       * Function to add empty black page in test
       * @param id The id
       * @param elements elements to add in pag
       * @returns Function to add blac page in test
       */
      addPage(id: string, ...elements: any[]): Chainable<Document>;
      /**
       * Function to add presentation page in test
       * @param title The title of tests
       * @param project The project of tests
       * @param version Theverion of project
       * @param author The author of project
       * @returns The added page
       */
      addPresentation(
        title: string,
        project?: string,
        version?: string,
        author?: string
      ): Cypress.Chainable<Document>;
      /**
       * Function to ad value in basic field
       * @param id The id of component
       * @param value value The value to set in component
       * @returns The Chainable Cypress type + check
       */

      basicInputType(
        id: string,
        value: string
      ): Cypress.Chainable<JQuery<HTMLElement>>;
      /**
       * Custom command to remove cypress left test list section
       * @example cy.clearViewport()
       */
      clearViewport(): Chainable<JQuery<HTMLBodyElement>>;

      /**
       * Custom command to amount react component in component test
       * @param jsx The component to mount
       * @param options
       * @param rerenderKey
       */
      mount(jsx: React.ReactNode, options: MountOptions = {}, rerenderKey?: string): Cypress.Chainable<MountReturn>;
    }
  }
}
