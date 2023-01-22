import React from "react";
import Hangman from "../../Hangman/Hangman";

type Props = {
  id: string;
};

function Home({ id }: Props) {
  return (
    <div id={id}>
      <p>Home</p>
      <Hangman width={500} height={500} quantity={11} />
    </div>
  );
}

export default Home;
