import React, { SyntheticEvent, useEffect } from "react";
import style from "./Keyboard.scss";

/**
 *
 */
type Props = {
  /**
   *
   */
  disabledLetters?: string;
  /**
   *
   */
  disposition: "azerty" | "querty";
  /**
   *
   * @param letter
   * @returns
   */
  onSelect?: (letter: string) => void;
};

/**
 *
 */
const AZERTY: string[] = ["AZERTYUIOP", "QSDFGHJKLM", "WXCVBN"];

/**
 *
 */
const QWERTY: string[] = ["QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"];

/**
 *
 * @param selectHandler
 * @returns
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
 *
 * @param selectHandler
 * @returns
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
 *
 * @param param0
 * @returns
 */
function Keyboard({ disposition, disabledLetters, onSelect }: Props) {
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
