import * as React from "react";
import * as ReactDOM from "react-dom";

import reportWebVitals from "./reportWebVitals";

import Header from "./components/Header/Header";
import Main from "./components/Main/Main";
import "./index.scss";

function App(): JSX.Element {
  return (
    <>
      <Header title="Hello" />
      <span>Ceci non </span>&nbsp;<span>est mon site</span>
      <Main />
    </>
  );
}

ReactDOM.render(<App />, document.getElementById("root"));
reportWebVitals(console.log);
