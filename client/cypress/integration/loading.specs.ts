function addPage(id : string, ...elements : any[]): void {
  cy.document().then($document => {
    cy.get('body').then(($div) => {
      function add(){
        const page = $document.createElement("div");
        page.id = `cypress-application-${id}`;
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

        elements.forEach((element) => page.append(element));
        return page;
      }
      $div.empty();
      $div.append(add);
    });
  });
}

async function addPresentation(): Promise<void> {
  const $document = await cy.document();
  const title = $document.createElement("h1");
  title.textContent = "Hangman Game";

  const subTitle = $document.createElement("h2");
  subTitle.textContent = "Démonstration";

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

  await addPage('pres', title, subTitle, version, date, author);
}

async function addEnd(): Promise<void> {
  const $document = await cy.document();
  const title = $document.createElement("h1");
  title.textContent = "Hangman Game";
  await addPage('end', title);
}

describe('Load presentation page', () => {
  it('load first page', () => {
    cy.clearViewport();
    cy.visit('/');
   
    addPresentation();

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

    addEnd();
  });
});
