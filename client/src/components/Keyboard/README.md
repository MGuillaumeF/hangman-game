The component **Keyboard**

```tsx
/**
 * Function to build the letter select handler
 * @param currentLetters The list of current letter aleardy used
 * @param updater The function to call to update selected letters list
 * @returns the letter select handler
 */
const onSelectBuilder = (currentLetters, updater) => {
  return (letter) => {
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
const dispositionUpdate = (disposition, setDisposition) => {
  return () => setDisposition(!disposition);
};

function CallerComponent() {
  const [disabledLetters, setDisabledLetters] = React.useState("");
  const [disposition, setDisposition] = React.useState(false);
  return (
    <React.Fragment>
      <p id={`used-letters`}>Used letters : {disabledLetters}</p>

      <Keyboard
        id={"styleguidist"}
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
    </React.Fragment>
  );
}

<CallerComponent />;
```
