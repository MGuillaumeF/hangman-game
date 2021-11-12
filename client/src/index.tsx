import * as React from "react";
import * as ReactDOM from "react-dom";

import Header from "./components/Header/Header";
import Main from "./components/Main/Main";
import "./index.scss";

function App(): JSX.Element {
  return (
    <>
      <Header title="Ceci est mon site" />
      <Main />
    </>
  );
}

ReactDOM.render(<App />, document.getElementById("root"));
