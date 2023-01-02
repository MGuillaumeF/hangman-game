"use strict";
/**
 * Custom command to remove cypress left test list section
 * @example cy.clearViewport()
 */
Cypress.Commands.add("clearViewport", function () {
    var runnerContainer = window.parent.document.getElementsByClassName("iframes-container")[0];
    if (runnerContainer) {
        runnerContainer.setAttribute("style", "left: 0; top: 0; width: 100%; height: 100%;");
    }
    var sizeContainer = window.parent.document.getElementsByClassName("size-container")[0];
    if (sizeContainer) {
        sizeContainer.setAttribute("style", "");
    }
    var sidebar = window.parent.document.getElementsByClassName("reporter-wrap")[0];
    if (sidebar) {
        sidebar.setAttribute("style", "opacity: 0");
    }
    var header = window.parent.document.querySelector(".runner.container header");
    if (header) {
        header.setAttribute("style", "opacity: 0");
    }
    return cy.get("body");
});
var AUTHOR = "MGuillaumeF";
var PROJECT = "Hangman Game";
var VERSION = "v0.1.0";
/**
 * Function to add empty black page in test
 * @param id The id
 * @param elements elements to add in pag
 * @returns Function to add blac page in test
 */
Cypress.Commands.add("addPage", function (id) {
    var elements = [];
    for (var _i = 1; _i < arguments.length; _i++) {
        elements[_i - 1] = arguments[_i];
    }
    return cy.document().then(function ($document) {
        cy.get("body").then(function ($div) {
            function add() {
                var page = $document.createElement("div");
                page.id = "cypress-application-".concat(id);
                page.style.backgroundColor = "#333";
                page.style.color = "#FFF";
                page.style.height = "100vh";
                page.style.width = "100vw";
                page.style.position = "fixed";
                page.style.display = "flex";
                page.style.alignItems = "center";
                page.style.justifyContent = "center";
                page.style.flexDirection = "column";
                page.style.zIndex = "3000000";
                elements.forEach(function (element) { return page.append(element); });
                return page;
            }
            $div.empty();
            $div.append(add);
        });
    });
});
/**
 * Function to add presentation page in test
 * @param title The title of tests
 * @param project The project of tests
 * @param version Theverion of project
 * @param author The author of project
 * @returns The added page
 */
Cypress.Commands.add("addPresentation", function (title, project, version, author) {
    if (project === void 0) { project = PROJECT; }
    if (version === void 0) { version = VERSION; }
    if (author === void 0) { author = AUTHOR; }
    return cy.document().then(function ($document) {
        var mainTitle = $document.createElement("h1");
        mainTitle.textContent = project;
        var subTitle = $document.createElement("h2");
        subTitle.textContent = title;
        var pVersion = $document.createElement("p");
        pVersion.textContent = version;
        var date = $document.createElement("span");
        // date.textContent = new Date().toISOString().split("T")[0];
        date.style.position = "fixed";
        date.style.bottom = "0.5em";
        date.style.right = "0.5em";
        var sAuthor = $document.createElement("span");
        sAuthor.textContent = author;
        sAuthor.style.position = "fixed";
        sAuthor.style.top = "0.5em";
        sAuthor.style.left = "0.5em";
        return cy.addPage("pres", mainTitle, subTitle, pVersion, date, sAuthor);
    });
});
/**
 * Function to add End page in test displaying
 * @returns The added page
 */
Cypress.Commands.add("addEnd", function () {
    return cy.document().then(function ($document) {
        var title = $document.createElement("h1");
        title.textContent = "FIN";
        return cy.addPage("end", title);
    });
});
/**
 * Function to ad value in basic field
 * @param id The id of component
 * @param value value The value to set in component
 * @returns The Chainable Cypress type + check
 */
Cypress.Commands.add("basicInputType", function (id, value) {
    cy.get("label[for=".concat(id, "]")).click();
    return cy.get("#".concat(id)).click().type(value).should("have.value", value);
});
