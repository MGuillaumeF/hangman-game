export default class HangmanError extends Error {
  // add specificity to debug
  public readonly name = "HangmanError";
  private static readonly date = Date.now();
  constructor(message: string, options: any) {
    // call parent constructor
    super(message, options);

    // error stack for V8
    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, HangmanError);
    }
  }
}
