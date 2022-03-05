/* eslint-disable react-perf/jsx-no-new-function-as-prop */
import * as React from "react";
import * as ReactDOM from "react-dom";

import reportWebVitals from "./reportWebVitals";

import Header from "./components/Header/Header";
import Main from "./components/Main/Main";
import "./index.scss";
import "./i18n";

import { useTranslation } from "react-i18next";

const scopeStyle : React.CSSProperties = {display : 'flex', flexDirection : 'column', justifyContent : "center", alignItems: "center"}
const formStyle : React.CSSProperties= {width : '10em', marginTop:"10em"}
const fieldsetStyle : React.CSSProperties = {display : 'flex', flexDirection : 'column'}
const langStyle : React.CSSProperties = {position : 'absolute', bottom : '0.5em', right : '0.5em'}

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
      <div style={scopeStyle}>
        <form style={formStyle} method="POST" action="/sign-in">
          <fieldset style={fieldsetStyle}>
            <legend>{t("forms.sign-in.title")}</legend>
            <label htmlFor="forms.sign-in.fields.identifier">{t("forms.sign-in.fields.identifier.label")}</label>
            <input type="text" name="login" id="forms.sign-in.fields.identifier" required />
            <label htmlFor="forms.sign-in.fields.identifier">{t("forms.sign-in.fields.password.label")}</label>
            <input type="password" name="password" id="forms.sign-in.fields.password" required/>
            <input type="submit" value={String(t("forms.sign-in.fields.submit.label"))} />
          </fieldset>
        </form>
        <div style={langStyle}>
          <button id="lang-fr" onClick={changeLangBtnClick}>FR</button>
          <button id="lang-en" onClick={changeLangBtnClick}>EN</button>
        </div>
      </div>
      
    </>
  );
}

ReactDOM.render(<App />, document.getElementById("root"));
reportWebVitals(console.log);
