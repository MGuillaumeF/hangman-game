describe('Load presentation page', () => {
  it('load first page', () => {
    cy.clearViewport();
    cy.visit('/');
    cy.document().then($document => {
      cy.get('body').then(($div) => {
        function words(){
          var image = $document.createElement("div");
          image.id = "cypress-application-pres";
          image.textContent = "Hangman Game v0.1.0";
          image.style.backgroundColor = "#333";
          image.style.color = "#FFF";
          image.style.height = "100%";
          image.style.width = "100%";
          image.style.position = "fixed";
          image.style.display = "flex";
          image.style.alignItems = "middle";
          image.style.justifyContent = "center";
          image.style.zIndex = 3000000;
          return image;
        }
        $div.append(words);
      });
    });
    cy.wait(3000);
    cy.visit('/');
    cy.document().then($document => {
      cy.get('body').then(($div) => {
        function words(){
          var image = $document.createElement("div");
          image.id = "cypress-application-pres";
          image.textContent = "Hangman Game v0.1.0";
          image.style.backgroundColor = "#333";
          image.style.color = "#FFF";
          image.style.height = "100%";
          image.style.width = "100%";
          image.style.position = "absolute";
          image.style.display = "flex";
          image.style.alignItems = "middle";
          image.style.justifyContent = "center";
          image.style.zIndex = 3000000;
          return image;
        }
        $div.append(words);
      });
    });
    cy.wait(3000);
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
