/**
 * Custom application error
 */
export default class HangmanError extends Error {
  // add specificity to debug
  public cause?: Error;
  private static readonly date = Date.now();
  constructor(message: string, options?: { cause: Error }) {
    // call parent constructor
    // @ts-ignore
    super(message, options);

    // error stack for V8
    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, HangmanError);
    }
  }
}
