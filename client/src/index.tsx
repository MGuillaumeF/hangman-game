import structuredClone from "core-js/features/structured-clone";
import React, { useCallback } from "react";
import { render } from "react-dom";
import { useTranslation } from "react-i18next";
import { BrowserRouter, Link, Route, Routes } from "react-router-dom";
import "regenerator-runtime/runtime";
import Button from "./components/BasicButton/Button";
import SignIn from "./components/Forms/SignIn/SignIn";
import SignUp from "./components/Forms/SignUp/SignUp";
import AboutUs from "./components/pages/AboutUs/AboutUs";
import CGU from "./components/pages/CGU/CGU";
import Error404 from "./components/pages/Errors/Error404";
import Home from "./components/pages/Home/Home";
import Messages from "./components/pages/Messages/Messages";
import Settings from "./components/pages/Settings/Settings";
import "./i18n";
import "./index.scss";
import reportWebVitals from "./reportWebVitals";

const scopeStyle: React.CSSProperties = {
  display: "flex",
  flexDirection: "column",
  justifyContent: "center",
  alignItems: "center"
};
const langStyle: React.CSSProperties = {
  position: "absolute",
  bottom: "0.5em",
  right: "0.5em"
};
const signInStyle: React.CSSProperties = {
  position: "absolute",
  top: "0.5em",
  right: "0.5em"
};

console.log(structuredClone(signInStyle));

const changeLangBtnClick = (
  event: React.MouseEvent<HTMLButtonElement>,
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  i18n: any
): void => {
  const currentBtnText = String(
    event?.currentTarget?.textContent
  ).toLowerCase();
  if (
    i18n &&
    typeof i18n === "object" &&
    "changeLanguage" in i18n &&
    ["fr", "en"].includes(currentBtnText)
  ) {
    i18n.changeLanguage(currentBtnText);
  }
};

function App(): JSX.Element {
  const { i18n, t } = useTranslation();

  const onLanguageClick = useCallback(
    (event: React.MouseEvent<HTMLButtonElement>) => {
      changeLangBtnClick(event, i18n);
    },
    [i18n]
  );

  return (
    <BrowserRouter>
      <div style={scopeStyle}>
        <span style={signInStyle}>
          <Link id="PAGES_SIGN_IN_LINK" to="sign-in">
            {String(t("FORMS.SIGN_IN.TITLE"))}
          </Link>
          &nbsp;/&nbsp;
          <Link id="PAGES_SIGN_UP_LINK" to="sign-up">
            {String(t("FORMS.SIGN_UP.TITLE"))}
          </Link>
        </span>
        <Routes>
          <Route path="*" element={<Error404 />}></Route>
          <Route path="/" element={<Home id="page-home" />}></Route>

          <Route path="about" element={<AboutUs id="page-about-us" />}></Route>
          <Route path="cgu" element={<CGU id="page-cgu" />}></Route>
          <Route path="sign-in" element={<SignIn id="SignIn" />}></Route>
          <Route path="sign-up" element={<SignUp id="SignUp" />}></Route>
          <Route
            path="messages"
            element={<Messages id="page-messages" />}
          ></Route>
        </Routes>
        <Settings id="PAGES_SETTINGS_TITLE" />
        <div style={langStyle}>
          <Button
            type="button"
            level="secondary"
            id="lang-fr"
            onClick={onLanguageClick}
          >
            FR
          </Button>
          <Button
            type="button"
            level="secondary"
            id="lang-en"
            onClick={onLanguageClick}
          >
            EN
          </Button>
        </div>
      </div>
    </BrowserRouter>
  );
}

render(<App />, document.getElementById("root"));
reportWebVitals(console.log);
