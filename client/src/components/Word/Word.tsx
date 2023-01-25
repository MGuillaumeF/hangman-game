import React from "react";
import style from "./Word.scss";
/**
 *
 */
type Props = {
  /**
   *
   */
  usedLetters: string;
  /**
   *
   */
  word: string;
};

/**
 *
 * @param param0
 * @returns
 */
function Word({ word, usedLetters }: Props): JSX.Element {
  return (
    <div className={style.Word}>
      {word
        .toLocaleUpperCase()
        .split("")
        .map((letter: string, index: number): JSX.Element => {
          return (
            <div
              className={
                usedLetters.includes(letter) ? style.letter : style.empty
              }
              key={index}
            >
              {usedLetters.includes(letter) ? letter : "_"}
            </div>
          );
        })}
    </div>
  );
}

export default Word;
