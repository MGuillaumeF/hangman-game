import { expect } from "chai";
import Word from "../../src/model/Word";

describe("Word Model Object Test Suite", function () {
  it("Test parse method", function () {
   
      const myWord = Word.parse({
        name : "hello",
        definitions : ["salutation word"]
      });
      expect(myWord instanceof Word).to.equal(
        true,
        "Test word parser object is word instance"
      );
      if (myWord instanceof Word) {
        console.info("word parsed", myWord);
        
        expect(myWord.name).to.be.a(
          "string",
          "test stack attribute is present and is type string"
        );
        expect(myWord.definitions).to.be.a(
          "array",
          "test definition attribute is present and is type array"
        );
        
      }
  });
});
