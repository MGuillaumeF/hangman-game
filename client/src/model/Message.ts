/**
 * @filename Message.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { User } from "./User";
import { Chat } from "./Chat";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class Message extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        content: {
          mandatory: false,
          min_length: 1,
          type: "string"
        },
        send_date: {
          mandatory: true,
          type: "Date"
        }
      }
    };
  }

  private _content: string | undefined;

  private _sender: User | undefined;

  private _sendDate: Date | undefined;

  private _chat: Chat | undefined;

  /**
   * @brief Set the content of object
   *
   * @param value The new content value of object
   */
  public set content(value: string | undefined) {
    this._content = value;
  }
  /**
   * @brief Get the content of object
   *
   * @return string | undefined the content of object
   */
  public get content(): string | undefined {
    return this._content;
  }
  /**
   * @brief Set the sender of object
   *
   * @param value The new sender value of object
   */
  public set sender(value: User | undefined) {
    this._sender = value;
  }
  /**
   * @brief Get the sender of object
   *
   * @return User | undefined the sender of object
   */
  public get sender(): User | undefined {
    return this._sender;
  }
  /**
   * @brief Set the send_date of object
   *
   * @param value The new send_date value of object
   */
  public set sendDate(value: Date | undefined) {
    this._sendDate = value;
  }
  /**
   * @brief Get the send_date of object
   *
   * @return Date | undefined the sendDate of object
   */
  public get sendDate(): Date | undefined {
    return this._sendDate;
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
  /**
   * method to get errors of objects
   * @returns the list of model constraint error
   */
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      ...Validator.checkStringProperty(
        Message.getConstraintes().content,
        "content",
        this.content
      )
    );
    if (this.sender !== undefined) {
      errors.push(...this.sender.getErrors());
    }
    if (this.chat !== undefined) {
      errors.push(...this.chat.getErrors());
    }
    return errors;
  }
  /**
   * @brief method to convert object to JSON object
   * @returns the object json representation of object instance
   */
  public toJSON(): any {
    const { content, sender, sendDate, chat } = this;
    return {
      ...super.toJSON(),
      content,
      sender: sender !== undefined ? sender.toJSON() : undefined,
      sendDate,
      chat: chat !== undefined ? chat.toJSON() : undefined
    };
  }

  /**
   * @brief method to generate convertor of any object to instance of class
   * @param data The data to convert to instance
   * @returns The instance of converted object
   */
  public static parse(data: any): Message {
    const obj = RootModelObject.parseMetaData<Message>(new Message(), data);
    if (typeof data === "object") {
      if (data["content"] !== undefined) {
        if (typeof data["content"] === "string") {
          obj.content = data["content"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["sender"] !== undefined) {
        if (typeof data["sender"] === "object") {
          obj.sender = User.parse(data["sender"]);
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["send_date"] !== undefined) {
        if (data["send_date"] instanceof Date) {
          obj.sendDate = data["send_date"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["chat"] !== undefined) {
        if (typeof data["chat"] === "object") {
          obj.chat = Chat.parse(data["chat"]);
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
