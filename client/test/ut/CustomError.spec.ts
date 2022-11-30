import assert from "assert";
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
    // TODO remove sample test case
    assert.equal([1, 2, 3].indexOf(4), -1);

    try {
      fakeParser();
    } catch (e) {
      console.error("error raised un catch", e);
      assert.equal(true, e instanceof HangmanError);
      if (e instanceof HangmanError) {
        expect(e.message).to.equal("Custom parsing error");
        expect(e?.stack).to.be.a("string");
        if (e?.stack) {
          // test child error class name appear in stack trace
          expect(e!.stack.includes("HangmanError")).to.equal(true);
          // test child rethrow line appear in stack trace
          expect(e!.stack.includes("CustomError.spec.ts:9:13")).to.equal(true);
          expect(e!.stack.includes("CustomError.spec.ts:6:10")).to.equal(true);
          expect(
            e!.stack.includes(
              "SyntaxError: Unexpected token o in JSON at position 1"
            )
          ).to.equal(true);
          expect(
            e!.stack.includes("HangmanError: Custom parsing error")
          ).to.equal(true);
        }
      }
    }
  });
});
