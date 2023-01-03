import React from 'react';
import Button from '../../src/components/BasicButton/Button'

it('uses custom text for the button label', () => {
  const compo = <><Button id="sample" type="button">Click me!</Button></>
  cy.mount(compo)
  cy.get('button').should('contains.text', 'Click me!')
})
