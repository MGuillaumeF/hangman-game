import React from "react";

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
    <p>
      {word
        .toLocaleUpperCase()
        .split("")
        .map((letter: string, index: number): JSX.Element => {
          return (
            <span key={index}>
              {usedLetters.includes(letter) ? letter : "_"}
            </span>
          );
        })}
    </p>
  );
}

export default Word;
