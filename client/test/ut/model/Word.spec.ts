import { expect } from "chai";
import { Word } from "../../../src/model/Word";

const WORD_EXAMPLE = {
  name: "hello",
  definitions: ["salutation word"],
  dictionary: {
    country_code: "USA",
    name: "AMERICAN",
    words: [
      {
        id: 1,
        created_by: "anonymous",
        created_at: new Date(),
        updated_at: new Date(),
        updated_by: "anonymous",
        version: 1,
        name: "world",
        definitions: ["space of all", "scope of another def"]
      }
    ]
  }
};

describe("Word Model Object Test Suite", function () {
  it("Test parse method", function () {
    const myWord = Word.parse(WORD_EXAMPLE);
    console.info("word parsed", myWord);
    expect(myWord instanceof Word).to.equal(
      true,
      "Test word parser object is word instance"
    );
    if (myWord instanceof Word) {
      expect(myWord.name).to.be.a(
        "string",
        "test name attribute is present and is type string"
      );
      expect(myWord.definitions).to.be.a(
        "array",
        "test definitions attribute is present and is type array"
      );
      expect(myWord.name).to.equal(
        "hello",
        "test name attribute is present and has good value"
      );
      if (Array.isArray(myWord?.definitions)) {
        expect(myWord.definitions[0]).to.equal(
          "salutation word",
          "test definitions attribute is present and has good value in first case"
        );
      }
    }
  });

  it("Test toJson method", function () {
    const myWord: any = Word.parse(WORD_EXAMPLE).toJSON();
    console.info("word json object", myWord);

    expect(myWord?.name).to.be.a(
      "string",
      "test name attribute is present and is type string"
    );
    expect(myWord?.definitions).to.be.a(
      "array",
      "test definitions attribute is present and is type array"
    );
    expect(myWord?.name).to.equal(
      "hello",
      "test name attribute is present and has good value"
    );
    if (Array.isArray(myWord?.definitions)) {
      expect(myWord.definitions[0]).to.equal(
        "salutation word",
        "test definitions attribute is present and has good value in first case"
      );
    }
  });

  it("Test cyclicly conservation parse/toJson method", function () {
    const myWord: any = Word.parse(WORD_EXAMPLE).toJSON();

    expect(myWord).to.be.equal(
      WORD_EXAMPLE,
      "test parse to json object are equals"
    );

  });
});
