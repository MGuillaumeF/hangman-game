/**
 * @filename Dictionary.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { Word } from "./Word";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class Dictionary extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        name: {
          mandatory: "true",
          min_length: "3",
          pattern: "^[a-zA-Z0-9]{3,}$",
          type: "string"
        },
        country_code: {
          mandatory: "true",
          min_length: "3",
          pattern: "^[a-zA-Z0-9]{3,}$",
          type: "string"
        }
      }
    };
  }

  private _name: string | undefined;

  private _countryCode: string | undefined;

  private _words: Word[] | undefined;

  /**
   * @brief Set the name of object
   *
   * @param value The new name value of object
   */
  public set name(value: string | undefined) {
    this._name = value;
  }
  /**
   * @brief Get the name of object
   *
   * @return string | undefined the name of object
   */
  public get name(): string | undefined {
    return this._name;
  }
  /**
   * @brief Set the country_code of object
   *
   * @param value The new country_code value of object
   */
  public set countryCode(value: string | undefined) {
    this._countryCode = value;
  }
  /**
   * @brief Get the country_code of object
   *
   * @return string | undefined the countryCode of object
   */
  public get countryCode(): string | undefined {
    return this._countryCode;
  }
  /**
   * @brief Set the words of object
   *
   * @param value The new words value of object
   */
  public set words(value: Word[] | undefined) {
    this._words = value;
  }
  /**
   * @brief Get the words of object
   *
   * @return Word[] | undefined the words of object
   */
  public get words(): Word[] | undefined {
    return this._words;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      ...Validator.checkStringProperty(
        Dictionary.getConstraintes().name,
        this.name
      )
    );
    errors.push(
      ...Validator.checkStringProperty(
        Dictionary.getConstraintes().country_code,
        this.countryCode
      )
    );
    return errors;
  }
  public toJSON() {
    const { name, countryCode, words } = this;
    return { name, countryCode, words: this.words.toJSON() };
  }

  public static parse(data: any): Dictionary {
    const obj = new Dictionary();
    if (typeof data === "object") {
      if (data["name"] !== undefined) {
        if (typeof data["name"] === "string") {
          obj.name = data["name"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["country_code"] !== undefined) {
        if (typeof data["country_code"] === "string") {
          obj.countryCode = data["country_code"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["words"] !== undefined) {
        if (typeof data["words"] === "word[]") {
          obj.words = data["words"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
