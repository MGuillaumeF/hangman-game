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

    cy.document().then($document => {
      cy.get('body').then(($div) => {
        function words(){
          var image = $document.createElement("div");
          image.textContent = "Hangman Game v0.1.0";
          image.backgroundColor = "#000";
          image.height = "100vh";
          image.width = "100%";
          return image
        }
        $div.append(words);
      });
    });
  });
});
