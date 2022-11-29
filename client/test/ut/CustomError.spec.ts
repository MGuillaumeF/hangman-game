import assert from "assert";
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
        assert.equal("Custom parsing error", e?.message);
      }
    }
  });
});
