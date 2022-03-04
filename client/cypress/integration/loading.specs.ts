describe('Load presentation page', () => {
  it('load first page', () => {
    cy.clearViewport();
    cy.visit('/');
    cy.document().then($document => {
      cy.get('body').then(($div) => {
        function presentation(){
          const page = $document.createElement("div");
          page.id = "cypress-application-pres";
          page.style.backgroundColor = "#333";
          page.style.color = "#FFF";
          page.style.height = "100%";
          page.style.width = "100%";
          page.style.position = "fixed";
          page.style.display = "flex";
          page.style.alignItems = "center";
          page.style.justifyContent = "center";
          page.style.flexDirection = "column";
          page.style.zIndex = 3000000;

          const title = $document.createElement("h1");
          title.textContent = "Hangman Game";
         // title.style.fontSize = "2.5em";

          const version = $document.createElement("p");
          version.textContent = "v0.1.0";

          const date = $document.createElement("span");
          date.textContent = new Date().toISOString().split('T')[0];
          date.style.position = "fixed";
          date.style.bottom = "0.5em";
          date.style.right = "0.5em";

          const author = $document.createElement("span");
          author.textContent = "MGuillaumeF";
          author.style.position = "fixed";
          author.style.top = "0.5em";
          author.style.left = "0.5em";

          page.append(title);
          page.append(version);
          page.append(date);
          page.append(author);
          return page;
        }
        $div.empty();
        $div.append(presentation);
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
