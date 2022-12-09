/**
 * Custom application error
 */
export default class HangmanError extends Error {
  /**
   * error name to get it when errro is raised
   */
  public readonly name = HangmanError.name;
  /**
   * error cause if error is a rethrow
   */
  public cause?: Error;
  /**
   * the date of error throw
   */
  private static readonly date = Date.now();
  /**
   * @param message the message of error
   * @param options the cause or error
   */
  constructor(message: string, options?: { cause: Error }) {
    // call parent constructor
    super(message, options);

    // error stack for V8
    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, HangmanError);
    }
  }
}
