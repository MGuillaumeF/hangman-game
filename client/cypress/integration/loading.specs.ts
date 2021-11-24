describe('Load presentation page', () => {
  it('load first page', () => {
    cy.visit('/');
    cy.contains('Ceci est mon site');
  });
});
