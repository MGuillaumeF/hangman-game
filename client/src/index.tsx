import structuredClone from "core-js/features/structured-clone";
import React, { useCallback, useState } from "react";
import { render } from "react-dom";
import { useTranslation } from "react-i18next";
import { BrowserRouter, Link, Route, Routes } from "react-router-dom";
import "regenerator-runtime/runtime";
import SignIn from "./components/Forms/SignIn/SignIn";
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
  right: "0.5em",
  display: "flex",
  flexDirection: "column",
  backgroundColor: "#777"
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
  const [displaySignInForm, setdisplaySignInForm] = useState(false);

  const onLanguageClick = useCallback(
    (event: React.MouseEvent<HTMLButtonElement>) => {
      changeLangBtnClick(event, i18n);
    },
    [i18n]
  );

  const onClickDisplaySignIn = useCallback(() => {
    setdisplaySignInForm(!displaySignInForm);
  }, [displaySignInForm]);

  return (
    <BrowserRouter>
      <div style={scopeStyle}>
        <div style={signInStyle}>
          <button onClick={onClickDisplaySignIn}>Connexion</button>
          {displaySignInForm ? <SignIn id="form-sign-in" /> : null}
        </div>
        <Routes>
          <Route path="*" element={<Error404 />}></Route>
          <Route path="/" element={<Home id="page-home" />}></Route>
          <Route
            path="settings"
            element={<Settings id="page-settings" />}
          ></Route>
          <Route path="about" element={<AboutUs id="page-about-us" />}></Route>
          <Route path="/cgu" element={<CGU id="page-cgu" />}></Route>
          <Route
            path="messages"
            element={<Messages id="page-messages" />}
          ></Route>
        </Routes>

        <div style={langStyle}>
          <Link id="PAGES_SETTINGS_TITLE" to="settings">
            {t("PAGES.SETTINGS.TITLE")}
          </Link>
          <button id="lang-fr" onClick={onLanguageClick}>
            FR
          </button>
          <button id="lang-en" onClick={onLanguageClick}>
            EN
          </button>
        </div>
      </div>
    </BrowserRouter>
  );
}

render(<App />, document.getElementById("root"));
reportWebVitals(console.log);
