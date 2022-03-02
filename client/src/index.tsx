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
  const changeLang = (lang: string) => {
    i18n.changeLanguage(lang);
  };

  const changeLangBtnClick = (
    event: React.MouseEvent<HTMLButtonElement>
  ): void => {
    const currentBtnText = String(
      event?.currentTarget?.textContent
    ).toLowerCase();
    if (["fr", "en"].includes(currentBtnText)) {
      changeLang(currentBtnText);
    }
  };
  return (
    <>
      <Header title="Ceci est mon site" />
      <Main />
      <form>
        <h2>{t("forms.sign-in.title")}</h2>
      </form>
      <button onClick={changeLangBtnClick}>FR</button>
      <button onClick={changeLangBtnClick}>EN</button>
    </>
  );
}

ReactDOM.render(<App />, document.getElementById("root"));
reportWebVitals(console.log);
