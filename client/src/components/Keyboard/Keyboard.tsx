import React, { SyntheticEvent, useEffect } from "react";
import style from "./Keyboard.scss";

/**
 * Properties to build Keyboard component
 */
type KeyboardProps = {
  /**
   * The list of letter to disable in keyboard
   */
  disabledLetters?: string;
  /**
   * The disposition of keyboard's letters
   */
  disposition: "azerty" | "querty";
  /**
   * The id prefix of letters [id]-keyboard-[letter]
   */
  id: string;
  /**
   * Function called when a letter is selected
   * @param letter The letter selected
   * @returns void
   */
  onSelect?: (letter: string) => void;
};

/**
 * The letters rows of AZERTY keyboard
 */
const AZERTY: string[] = ["AZERTYUIOP", "QSDFGHJKLM", "WXCVBN"];

/**
 * The letters rows of QWERTY keyboard
 */
const QWERTY: string[] = ["QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"];

/**
 * The function to build on click callback from the letter selection handler
 * @param selectHandler The function to call when a letter is selected
 * @returns The SyntheticEvent handler caller of letter select handler
 */
const onClickBuilder = (
  selectHandler: (letter: string) => void
): ((event: SyntheticEvent) => void) => {
  return (event: SyntheticEvent) => {
    if (event.currentTarget.textContent !== null) {
      selectHandler(event.currentTarget.textContent);
    }
  };
};

/**
 * The function to build on Keypress callback from the letter selection handler
 * @param selectHandler The function to call when a letter is selected
 * @returns The KeyboardEvent handler caller of letter select handler
 */
const onKeyDownListenerBuilder = (
  selectHandler: (letter: string) => void
): ((this: HTMLElement, event: KeyboardEvent) => void) => {
  return function (this: HTMLElement, event: KeyboardEvent): void {
    const letter = event.key.toLocaleUpperCase();
    if (/^[A-Z]$/i.test(letter)) {
      selectHandler(letter);
    }
  };
};

/**
 * Component of Keyboard entry
 * @param param0 @see KeyboardProps
 * @returns The JSX.Element component
 */
function Keyboard({ disposition, disabledLetters, id, onSelect }: KeyboardProps): JSX.Element {
  const letters = disposition === "azerty" ? AZERTY : QWERTY;
  const disabledLettersUpperCase = disabledLetters
    ? disabledLetters.toLocaleUpperCase()
    : "";
  useEffect((): (() => void) | undefined => {
    if (onSelect) {
      const handler = onKeyDownListenerBuilder(onSelect);
      document.body.addEventListener("keydown", handler, false);

      return () => {
        document.body.removeEventListener("keydown", handler);
      };
    } else {
      return;
    }
  }, [onSelect]);

  return (
    <div className={style.Keyboard}>
      {letters.map((row: string, index: number) => (
        <div key={index}>
          {row.split("").map((letter: string) => (
            <button
              id={`${id}-keyboard-${letter}`}
              disabled={disabledLettersUpperCase.includes(letter)}
              key={letter}
              onClick={onSelect ? onClickBuilder(onSelect) : undefined}
            >
              {letter}
            </button>
          ))}
        </div>
      ))}
    </div>
  );
}

export default Keyboard;
