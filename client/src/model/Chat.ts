/**
 * @filename Chat.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class Chat extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        name: {
          mandatory: "true",
          min_length: "3",
          pattern: "/^[a-zA-Z0-9]{3,}/$",
          type: "string"
        }
      }
    };
  }

  private _name: string | undefined;

  private _messages: message[] | undefined;

  private _members: user[] | undefined;

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
   * @brief Set the messages of object
   *
   * @param value The new messages value of object
   */
  public set messages(value: message[] | undefined) {
    this._messages = value;
  }
  /**
   * @brief Get the messages of object
   *
   * @return message[] | undefined the messages of object
   */
  public get messages(): message[] | undefined {
    return this._messages;
  }
  /**
   * @brief Set the members of object
   *
   * @param value The new members value of object
   */
  public set members(value: user[] | undefined) {
    this._members = value;
  }
  /**
   * @brief Get the members of object
   *
   * @return user[] | undefined the members of object
   */
  public get members(): user[] | undefined {
    return this._members;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      Validator.checkStringProperty(Chat.getConstraintes().name, this.name)
    );
    return errors;
  }
  public toJSON() {
    const { name, messages, members } = this;
    return JSON.stringify({ name, messages, members });
  }

  public static parse(data: any): Chat {
    const obj = new Chat();
    if (typeof data === "object") {
      if (data["name"] !== undefined) {
        if (typeof data["name"] === "string") {
          obj.name = data["name"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["messages"] !== undefined) {
        if (typeof data["messages"] === "message[]") {
          obj.messages = data["messages"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["members"] !== undefined) {
        if (typeof data["members"] === "user[]") {
          obj.members = data["members"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
