import structuredClone from "core-js/features/structured-clone";
import React, { useCallback } from "react";
import { render } from "react-dom";
import { useTranslation } from "react-i18next";
import { BrowserRouter, Link, Route, Routes } from "react-router-dom";
import "regenerator-runtime/runtime";
import "../i18n";
import "../index.scss";
import KeyboardBook from "./KeyboardBook";

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

function Book(): JSX.Element {
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
          <Link id="PAGES_KEYBOARD" to="book/keyboard-book">
            Keyboard Book
          </Link>
        </span>
        <Routes>
          <Route
            path="book/keyboard-book"
            element={<KeyboardBook id="keyboard-book" />}
          ></Route>
        </Routes>
      </div>
    </BrowserRouter>
  );
}

render(<Book />, document.getElementById("root"));
