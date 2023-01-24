import React, { useState } from "react";
import Hangman from "../../Hangman/Hangman";
import Keyboard from "../../Keyboard/Keyboard";
import Word from "../../Word/Word";

type Props = {
  id: string;
};

// const onChange =
//   (updater: React.Dispatch<React.SetStateAction<number>>) =>
//   (e: React.ChangeEvent<HTMLInputElement>) =>
//     updater(Number(e.target.value));

/**
 *
 * @param currentLetters
 * @param updater
 * @returns
 */
const onSelectBuilder = (
  currentLetters: string,
  updater: React.Dispatch<React.SetStateAction<string>>
): ((letter: string) => void) => {
  return (letter: string): void => {
    if (/^[A-Z]$/.test(letter) && !currentLetters.includes(letter)) {
      updater(`${currentLetters}${letter}`);
    }
  };
};

const WORD_TEST = "chaton";

/**
 *
 * @param param0
 * @returns
 */
function Home({ id }: Props) {
  const [disabledLetters, setDisabledLetters] = useState<string>("");

  return (
    <div id={id}>
      <p>Home</p>
      <p>Used letters : {disabledLetters}</p>
      {/* <input
        type="number"
        min={MIN_HANGMAN_VALUE}
        max={MAX_HANGMAN_VALUE}
        onChange={onChange(setErrorNumber)}
      /> */}
      <Word word={WORD_TEST} usedLetters={disabledLetters} />
      <Hangman
        width={500}
        height={500}
        quantity={
          disabledLetters
            .split("")
            .filter(
              (letter: string) =>
                !WORD_TEST.toLocaleUpperCase().includes(letter)
            ).length
        }
        color="#555555"
      />
      <Keyboard
        disposition="azerty"
        disabledLetters={disabledLetters}
        onSelect={onSelectBuilder(disabledLetters, setDisabledLetters)}
      />
    </div>
  );
}

export default Home;
