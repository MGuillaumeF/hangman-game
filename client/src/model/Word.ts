/**
 * @filename Word.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { Dictionary } from "./Dictionary";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class Word extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        name: {
          mandatory: true,
          min_length: 3,
          pattern: "^[a-zA-Z0-9]{3,}$",
          type: "string"
        },
        definitions: {
          mandatory: true,
          min_length: 3,
          type: "string"
        }
      }
    };
  }

  private _name: string | undefined;

  private _definitions: string[] | undefined;

  private _dictionary: Dictionary | undefined;

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
   * @brief Set the definitions of object
   *
   * @param value The new definitions value of object
   */
  public set definitions(value: string[] | undefined) {
    this._definitions = value;
  }
  /**
   * @brief Get the definitions of object
   *
   * @return string[] | undefined the definitions of object
   */
  public get definitions(): string[] | undefined {
    return this._definitions;
  }
  /**
   * @brief Set the dictionary of object
   *
   * @param value The new dictionary value of object
   */
  public set dictionary(value: Dictionary | undefined) {
    this._dictionary = value;
  }
  /**
   * @brief Get the dictionary of object
   *
   * @return Dictionary | undefined the dictionary of object
   */
  public get dictionary(): Dictionary | undefined {
    return this._dictionary;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      ...Validator.checkStringProperty(
        Word.getConstraintes().name,
        "name",
        this.name
      )
    );
    if (this.definitions !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.definitions.map((item) =>
            Validator.checkStringProperty(
              Word.getConstraintes().definitions,
              "definitions",
              item
            )
          )
        )
      );
    }
    if (this.dictionary !== undefined) {
      errors.push(...this.dictionary.getErrors());
    }
    return errors;
  }
  /**
   * @brief method to convert object to JSON object
   */
  public toJSON(): any {
    const { name, definitions, dictionary } = this;
    return {
      name,
      definitions,
      dictionary: dictionary !== undefined ? dictionary.toJSON() : undefined
    };
  }

  public static parse(data: any): Word {
    const obj = new Word();
    RootModelObject.parseMetaData<Word>(obj, data);
    if (typeof data === "object") {
      if (data["name"] !== undefined) {
        if (typeof data["name"] === "string") {
          obj.name = data["name"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["definitions"] !== undefined) {
        if (
          Array.isArray(data["definitions"]) &&
          data["definitions"].every((item: unknown) => typeof item === "string")
        ) {
          obj.definitions = data["definitions"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["dictionary"] !== undefined) {
        if (typeof data["dictionary"] === "object") {
          obj.dictionary = Dictionary.parse(data["dictionary"]);
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
