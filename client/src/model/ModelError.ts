/**
 * @filename ModelError.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { Validator } from "./Validator";

export class ModelError {
  protected static getConstraintes() {
    return {
      attribute_name: {
        mandatory: "true",
        type: "string"
      },
      message: {
        mandatory: "true",
        type: "string"
      }
    };
  }

  private _code: e_attribute_error | undefined;

  private _attributeName: string | undefined;

  private _message: string | undefined;

  /**
   * @brief Set the code of object
   *
   * @param value The new code value of object
   */
  public set code(value: e_attribute_error | undefined) {
    this._code = value;
  }
  /**
   * @brief Get the code of object
   *
   * @return e_attribute_error | undefined the code of object
   */
  public get code(): e_attribute_error | undefined {
    return this._code;
  }
  /**
   * @brief Set the attribute_name of object
   *
   * @param value The new attribute_name value of object
   */
  public set attributeName(value: string | undefined) {
    this._attributeName = value;
  }
  /**
   * @brief Get the attribute_name of object
   *
   * @return string | undefined the attributeName of object
   */
  public get attributeName(): string | undefined {
    return this._attributeName;
  }
  /**
   * @brief Set the message of object
   *
   * @param value The new message value of object
   */
  public set message(value: string | undefined) {
    this._message = value;
  }
  /**
   * @brief Get the message of object
   *
   * @return string | undefined the message of object
   */
  public get message(): string | undefined {
    return this._message;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      Validator.checkStringProperty(
        ModelError.getConstraintes().attribute_name,
        this.attributeName
      )
    );
    errors.push(
      Validator.checkStringProperty(
        ModelError.getConstraintes().message,
        this.message
      )
    );
    return errors;
  }
  public toJSON() {
    const { code, attributeName, message } = this;
    return JSON.stringify({ code, attributeName, message });
  }

  public static parse(data: any): ModelError {
    const obj = new ModelError();
    if (typeof data === "object") {
      if (data["code"] !== undefined) {
        if (typeof data["code"] === "e_attribute_error") {
          obj.code = data["code"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["attribute_name"] !== undefined) {
        if (typeof data["attribute_name"] === "string") {
          obj.attributeName = data["attribute_name"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["message"] !== undefined) {
        if (typeof data["message"] === "string") {
          obj.message = data["message"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
