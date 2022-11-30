import { expect } from "chai";
import HangmanError from "../../src/errors/HangmanError";

function fakeParser(): void {
  try {
    JSON.parse("not a json string");
  } catch (e) {
    if (e instanceof Error) {
      throw new HangmanError("Custom parsing error", { cause: e });
    }
  }
}

describe("Custom Error Test Suite", function () {
  it("should return -1 when the value is not present", function () {
    try {
      fakeParser();
    } catch (e) {
      console.error("error raised un catch", e);

      expect(e instanceof HangmanError).to.equal(
        true,
        "Test error is hangman error instance"
      );
      if (e instanceof HangmanError) {
        console.error("error message", e.message);
        console.error("error stack", e.stack);
        console.error("error string", e.toString());
        expect(e.message).to.equal(
          "Custom parsing error",
          "test message of custom error"
        );
        expect(e?.stack).to.be.a(
          "string",
          "test stack attribute is present and is type string"
        );
        if (e?.stack) {
          expect(e!.stack.includes("HangmanError")).to.equal(
            true,
            "test child error class name appear in stack trace"
          );
          expect(e!.stack.includes("CustomError.spec.ts:9")).to.equal(
            true,
            "test child rethrow error and custom error raised line appear in stack trace"
          );
          expect(e!.cause!.stack!.includes("CustomError.spec.ts:6")).to.equal(
            true,
            "test child rethrow error and original error raised line appear in stack trace"
          );
          expect(
            e!
              .cause!.stack!.includes("SyntaxError: Unexpected token o in JSON at position 1")
          ).to.equal(
            true,
            "test child rethrow error and original cause appear in stack trace"
          );
          expect(
            e!.stack.includes("HangmanError: Custom parsing error")
          ).to.equal(
            true,
            "test child rethrow error and custom cause appear in stack trace"
          );
        }
      }
    }
  });
});
