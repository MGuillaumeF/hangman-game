import React, { useState } from "react";
import Keyboard from "../components/Keyboard/Keyboard";

type Props = {
  id: string;
};

/**
 * Function to build the letter select handler
 * @param currentLetters The list of current letter aleardy used
 * @param updater The function to call to update selected letters list
 * @returns the letter select handler
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

/**
 * Function to build the dispistion updater handler
 * @param disposition The current dispostion of letters on keyboard
 * @param setDisposition The function to call to update disposotion
 * @returns The dispistion updater handler
 */
const dispositionUpdate = (
  disposition: boolean,
  setDisposition: React.Dispatch<React.SetStateAction<boolean>>
): (() => void) => {
  return () => setDisposition(!disposition);
};

/**
 *
 * @param param0
 * @returns
 */
function KeyboardBook({ id }: Props) {
  const [disabledLetters, setDisabledLetters] = useState<string>("");
  const [disposition, setDisposition] = useState<boolean>(false);

  return (
    <div id={id}>
      <p id={`${id}-used-letters`}>Used letters : {disabledLetters}</p>

      <Keyboard
        id="keyboard-component"
        disposition={disposition ? "qwerty" : "azerty"}
        disabledLetters={disabledLetters}
        onSelect={onSelectBuilder(disabledLetters, setDisabledLetters)}
      />
      <div id="command-panel">
        <button
          id="disposition-updater"
          onClick={dispositionUpdate(disposition, setDisposition)}
        >
          {disposition ? "qwerty" : "azerty"}
        </button>
      </div>
    </div>
  );
}

export default KeyboardBook;
