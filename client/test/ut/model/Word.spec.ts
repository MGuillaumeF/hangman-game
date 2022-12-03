import { expect } from "chai";
import { Word } from "../../../src/model/Word";

describe("Word Model Object Test Suite", function () {
  it("Test parse method", function () {
    const myWord = Word.parse({
      name: "hello",
      definitions: ["salutation word"],
      dictionary : {
        countryCode : "USA",
        name: "AMERICAN"
      }
    });
    expect(myWord instanceof Word).to.equal(
      true,
      "Test word parser object is word instance"
    );
    if (myWord instanceof Word) {
      console.info("word parsed", myWord);

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
    const myWord: any = Word.parse({
      name: "hello",
      definitions: ["salutation word"],
      dictionary : {
        countryCode : "USA",
        name: "AMERICAN"
      }
    }).toJSON();
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
});
