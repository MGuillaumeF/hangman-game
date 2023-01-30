import React from "react";
import style from "./Word.scss";

/**
 * Properties of Word to found displayer
 */
type WordProps = {
  /**
   * The id prefix of letter to found [id]-word-letter-[index]
   */
  id: string;
  /**
   * The string of used letters to display it
   */
  usedLetters: string;
  /**
   * The word to found
   */
  word: string;
};

/**
 * Component to display word found dashed output
 * @param param0 {@see WordProps}
 * @returns The JSX.Element of displayer
 */
function Word({ id, word, usedLetters }: WordProps): JSX.Element {
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
              id={`${id}-word-letter-${index}`}
            >
              {usedLetters.includes(letter) ? letter : "_"}
            </div>
          );
        })}
    </div>
  );
}

export default Word;
