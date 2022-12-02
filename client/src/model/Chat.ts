/**
 * @filename Chat.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { Message } from "./Message";
import { User } from "./User";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class Chat extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        name: {
          mandatory: true,
          min_length: 3,
          pattern: "^[a-zA-Z0-9]{3,}$",
          type: "string"
        }
      }
    };
  }

  private _name: string | undefined;

  private _messages: Message[] | undefined;

  private _members: User[] | undefined;

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
  public set messages(value: Message[] | undefined) {
    this._messages = value;
  }
  /**
   * @brief Get the messages of object
   *
   * @return Message[] | undefined the messages of object
   */
  public get messages(): Message[] | undefined {
    return this._messages;
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
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      ...Validator.checkStringProperty(
        Chat.getConstraintes().name,
        "name",
        this.name
      )
    );
    if (this.messages !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.messages.map((item) => item.getErrors())
        )
      );
    }
    if (this.members !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.members.map((item) => item.getErrors())
        )
      );
    }
    return errors;
  }
  /**
   * @brief method to convert object to JSON object
   */
  public toJSON(): any {
    const { name, messages, members } = this;
    return {
      name,
      messages:
        messages !== undefined
          ? messages.map((item) => item.toJSON())
          : undefined,
      members:
        members !== undefined ? members.map((item) => item.toJSON()) : undefined
    };
  }

  public static parse(data: any): Chat {
    const obj = RootModelObject.parseMetaData<Chat>(new Chat(), data);
    if (typeof data === "object") {
      if (data["name"] !== undefined) {
        if (typeof data["name"] === "string") {
          obj.name = data["name"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["messages"] !== undefined) {
        if (Array.isArray(data["messages"])) {
          obj.messages = data["messages"].map((item) => Message.parse(item));
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["members"] !== undefined) {
        if (Array.isArray(data["members"])) {
          obj.members = data["members"].map((item) => User.parse(item));
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
