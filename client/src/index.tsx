/* eslint-disable react-perf/jsx-no-new-function-as-prop */
import * as React from "react";
import * as ReactDOM from "react-dom";

import reportWebVitals from "./reportWebVitals";

import Header from "./components/Header/Header";
import Main from "./components/Main/Main";
import "./index.scss";
import "./i18n";

import { useTranslation } from "react-i18next";

function App(): JSX.Element {
  const { t, i18n } = useTranslation();

  const changeLangBtnClick = (
    event: React.MouseEvent<HTMLButtonElement>
  ): void => {
    const currentBtnText = String(
      event?.currentTarget?.textContent
    ).toLowerCase();
    if (["fr", "en"].includes(currentBtnText)) {
      i18n.changeLanguage(currentBtnText);
    }
  };
  return (
    <>
      <Header title="Ceci est mon site" />
      <Main />
      <form>
        <fieldset>
          <legend>{t("forms.sign-in.title")}</legend>
          <label htmlFor="forms.sign-in.fields.identifier">{t("forms.sign-in.fields.identifier.label")}</label>
          <input type="text" name="login" id="forms.sign-in.fields.identifier" />
          <label htmlFor="forms.sign-in.fields.identifier">{t("forms.sign-in.fields.password.label")}</label>
          <input type="password" name="password" id="forms.sign-in.fields.password" />
        </fieldset>
        <input type="submit" value={String(t("forms.sign-in.fields.submit.label"))} />
      </form>
      <button id="lang-fr" onClick={changeLangBtnClick}>FR</button>
      <button id="lang-en" onClick={changeLangBtnClick}>EN</button>
    </>
  );
}

ReactDOM.render(<App />, document.getElementById("root"));
reportWebVitals(console.log);
