"use strict";
// type definitions for Cypress object "cy"
/// <reference types="cypress" />
Object.defineProperty(exports, "__esModule", { value: true });
// type definitions for custom commands like "createDefaultTodos"
/// <reference types="../support" />
// check this file using TypeScript if available
// @ts-check
var chai_1 = require("chai");
// import * as specTitle from "cypress-sonarqube-reporter/specTitle";
// import { CyHttpMessages } from "cypress/types/net-stubbing";
describe("Page Access", function () {
    it("Visite page", function () {
        // cy.clearViewport();
        cy.visit("/");
        cy.addPresentation("Settings");
        cy.get("body");
    });
});
describe("Offlines Access", function () {
    it("Settings", function () {
        // cy.clearViewport();
        cy.visit("/");
        cy.addPresentation("Settings");
        cy.wait(2000);
        cy.visit("/");
        // all parameters are optional
        /*cy.toast("Change language to French", {
          blocking: true
        });*/
        cy.get("#lang-fr").click();
        cy.contains("Connexion");
        // all parameters are optional
        /*cy.toast("Change language to English", {
          blocking: true
        });*/
        cy.get("#lang-en").click();
        cy.contains("Sign In");
        cy.get("#PAGES_SETTINGS_TITLE").click();
        // all parameters are optional
        /*cy.toast("Display menu", {
          blocking: true
        });*/
        cy.addEnd();
    });
});
describe("Subscription", function () {
    it("With Success", function () {
        // cy.clearViewport();
        cy.visit("/");
        cy.addPresentation("Subscription - with success");
        cy.wait(2000);
        cy.visit("/");
        cy.intercept("POST", "/api/v1/user/sign-up", function (req
        // : CyHttpMessages.IncomingHttpRequest
        ) {
            var _a, _b, _c, _d;
            (0, chai_1.expect)((_a = req.body) === null || _a === void 0 ? void 0 : _a.email).to.equal("mguillaumef@draft.com");
            (0, chai_1.expect)((_b = req.body) === null || _b === void 0 ? void 0 : _b.login).to.equal("mguillaumef");
            (0, chai_1.expect)((_c = req.body) === null || _c === void 0 ? void 0 : _c.password).to.equal("DROWssap987");
            (0, chai_1.expect)((_d = req.body) === null || _d === void 0 ? void 0 : _d.confirm).to.equal("DROWssap987");
        }).as("subscribe");
        cy.get("#PAGES_SIGN_UP_LINK").click();
        cy.basicInputType("FORMS_SIGN_UP_FIELDS_EMAIL", "mguillaumef@draft.com");
        cy.basicInputType("FORMS_SIGN_UP_FIELDS_IDENTIFIER", "mguillaumef");
        cy.basicInputType("FORMS_SIGN_UP_FIELDS_PASSWORD", "DROWssap987");
        cy.basicInputType("FORMS_SIGN_UP_FIELDS_CONFIRM", "DROWssap987").type("{enter}");
        cy.wait("@subscribe");
        cy.addEnd();
    });
});
describe("Login", function () {
    it("With Success", function () {
        // cy.clearViewport();
        cy.visit("/");
        cy.addPresentation("Login - with success");
        cy.wait(2000);
        cy.visit("/");
        cy.intercept("POST", "/api/v1/user/sign-in", function (req
        // : CyHttpMessages.IncomingHttpRequest
        ) {
            var _a, _b;
            (0, chai_1.expect)((_a = req.body) === null || _a === void 0 ? void 0 : _a.login).to.equal("mguillaumef");
            (0, chai_1.expect)((_b = req.body) === null || _b === void 0 ? void 0 : _b.password).to.equal("DROWssap987");
        }).as("login");
        cy.get("#PAGES_SIGN_IN_LINK").click();
        cy.basicInputType("FORMS_SIGN_IN_FIELDS_IDENTIFIER", "mguillaumef");
        cy.basicInputType("FORMS_SIGN_IN_FIELDS_PASSWORD", "DROWssap987").type("{enter}");
        cy.wait("@login");
        cy.addEnd();
    });
});
describe("Errors pages", function () {
    it("Not Found", function () {
        // cy.clearViewport();
        cy.visit("/");
        cy.addPresentation("Not Found");
        cy.wait(2000);
        cy.visit("/badPage");
        cy.contains("Error404");
        cy.addEnd();
    });
});
