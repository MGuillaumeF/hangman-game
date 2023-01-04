import React from 'react';
import Button from '../../src/components/BasicButton/Button'


const CompoToTest = () : React.ReactNode => {
  return <><Button id="sample" type="button">Click me</Button></>
}


it('uses custom text for the button label', () => {
  cy.mount(<CompoToTest />)
  cy.get('button').should('contains.text', 'Click me')
})
