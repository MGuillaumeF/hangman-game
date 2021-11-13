import * as React from "react";
import "./Header.scss";

function Header(props: { title: string | number }): JSX.Element {
  return (
    <header className="Header">
      <h1>{props.title}</h1>
    </header>
  );
}

export default Header;
