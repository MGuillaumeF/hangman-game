/**
 * @filename Party.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { User } from "./User";
import { Chat } from "./Chat";
import { Word } from "./Word";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class Party extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        start_date: {
          mandatory: "true",
          type: "Date"
        },
        end_date: {
          mandatory: "false",
          type: "Date"
        }
      }
    };
  }

  private _startDate: Date;

  private _endDate: Date;

  private _win: boolean;

  private _members: User[];

  private _chat: Chat;

  private _word: Word;

  /**
   * @brief Set the start_date of object
   *
   * @param value The new start_date value of object
   */
  public set startDate(value: Date) {
    this._startDate = value;
  }
  /**
   * @brief Get the start_date of object
   *
   * @return Date the startDate of object
   */
  public get startDate(): Date {
    return this._startDate;
  }
  /**
   * @brief Set the end_date of object
   *
   * @param value The new end_date value of object
   */
  public set endDate(value: Date) {
    this._endDate = value;
  }
  /**
   * @brief Get the end_date of object
   *
   * @return Date the endDate of object
   */
  public get endDate(): Date {
    return this._endDate;
  }
  /**
   * @brief Set the win of object
   *
   * @param value The new win value of object
   */
  public set win(value: boolean) {
    this._win = value;
  }
  /**
   * @brief Get the win of object
   *
   * @return boolean the win of object
   */
  public get win(): boolean {
    return this._win;
  }
  /**
   * @brief Set the members of object
   *
   * @param value The new members value of object
   */
  public set members(value: User[]) {
    this._members = value;
  }
  /**
   * @brief Get the members of object
   *
   * @return User[] the members of object
   */
  public get members(): User[] {
    return this._members;
  }
  /**
   * @brief Set the chat of object
   *
   * @param value The new chat value of object
   */
  public set chat(value: Chat) {
    this._chat = value;
  }
  /**
   * @brief Get the chat of object
   *
   * @return Chat the chat of object
   */
  public get chat(): Chat {
    return this._chat;
  }
  /**
   * @brief Set the word of object
   *
   * @param value The new word value of object
   */
  public set word(value: Word) {
    this._word = value;
  }
  /**
   * @brief Get the word of object
   *
   * @return Word the word of object
   */
  public get word(): Word {
    return this._word;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];

    return errors;
  }
  public toJSON() {
    const { startDate, endDate, win, members, chat, word } = this;
    return JSON.stringify({ startDate, endDate, win, members, chat, word });
  }

  public static parse(data: any): Party {
    const obj = new Party();
    if (typeof data === "object") {
      if (data["start_date"] !== undefined) {
        if (typeof data["start_date"] === "date") {
          obj.startDate = data["start_date"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["end_date"] !== undefined) {
        if (typeof data["end_date"] === "date") {
          obj.endDate = data["end_date"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["win"] !== undefined) {
        if (typeof data["win"] === "bool") {
          obj.win = data["win"];
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
      if (data["word"] !== undefined) {
        if (typeof data["word"] === "word") {
          obj.word = data["word"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
