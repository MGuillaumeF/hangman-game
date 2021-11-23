describe('Load presentation page', () => {
  it('load first page', () => {
    cy.visit('http://localhost:3000');

    cy.contains('Ceci est mon site');
  });
});
