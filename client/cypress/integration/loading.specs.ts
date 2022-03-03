describe('Load presentation page', () => {
  it('load first page', () => {
    cy.clearViewport();
    cy.visit('/');
    cy.contains('Ceci est mon site');
    // all parameters are optional
    cy.toast('Loading', {
      duration: 3000,
      blocking: true,
    });
    cy.get("#lang-fr").click();
    cy.contains("Connexion");
    cy.get("#lang-en").click();
    cy.contains("Login");
  });
});
