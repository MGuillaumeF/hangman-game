/**
 * @filename Team.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { User } from "./User";
import { Chat } from "./Chat";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class Team extends RootModelObject {
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

  private _members: User[] | undefined;

  private _chat: Chat | undefined;

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
   * @brief Set the members of object
   *
   * @param value The new members value of object
   */
  public set members(value: User[] | undefined) {
    this._members = value;
  }
  /**
   * @brief Get the members of object
   *
   * @return User[] | undefined the members of object
   */
  public get members(): User[] | undefined {
    return this._members;
  }
  /**
   * @brief Set the chat of object
   *
   * @param value The new chat value of object
   */
  public set chat(value: Chat | undefined) {
    this._chat = value;
  }
  /**
   * @brief Get the chat of object
   *
   * @return Chat | undefined the chat of object
   */
  public get chat(): Chat | undefined {
    return this._chat;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      ...Validator.checkStringProperty(Team.getConstraintes().name, this.name)
    );
    return errors;
  }
  public toJSON() {
    const { name, members, chat } = this;
    return JSON.stringify({ name, members, chat });
  }

  public static parse(data: any): Team {
    const obj = new Team();
    if (typeof data === "object") {
      if (data["name"] !== undefined) {
        if (typeof data["name"] === "string") {
          obj.name = data["name"];
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
      if (data["chat"] !== undefined) {
        if (typeof data["chat"] === "chat") {
          obj.chat = data["chat"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
