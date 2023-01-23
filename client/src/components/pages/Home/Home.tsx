import React, { useState } from "react";
import Hangman, {
  MAX_HANGMAN_VALUE,
  MIN_HANGMAN_VALUE
} from "../../Hangman/Hangman";

type Props = {
  id: string;
};

const onChange =
  (updater: React.Dispatch<React.SetStateAction<number>>) =>
  (e: React.ChangeEvent<HTMLInputElement>) =>
    updater(Number(e.target.value));

function Home({ id }: Props) {
  const [errorNumber, setErrorNumber] = useState(0);

  return (
    <div id={id}>
      <p>Home</p>

      <input
        type="number"
        min={MIN_HANGMAN_VALUE}
        max={MAX_HANGMAN_VALUE}
        onChange={onChange(setErrorNumber)}
      />
      <Hangman
        width={500}
        height={500}
        quantity={errorNumber}
        color="#555555"
      />
    </div>
  );
}

export default Home;
