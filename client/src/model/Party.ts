/**
 * @filename Party.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
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

  private _startDate: Date | undefined;

  private _endDate: Date | undefined;

  private _win: boolean | undefined;

  private _members: user[] | undefined;

  private _chat: chat | undefined;

  private _word: word | undefined;

  /**
   * @brief Set the start_date of object
   *
   * @param value The new start_date value of object
   */
  public set startDate(value: Date | undefined) {
    this._startDate = value;
  }
  /**
   * @brief Get the start_date of object
   *
   * @return Date | undefined the startDate of object
   */
  public get startDate(): Date | undefined {
    return this._startDate;
  }
  /**
   * @brief Set the end_date of object
   *
   * @param value The new end_date value of object
   */
  public set endDate(value: Date | undefined) {
    this._endDate = value;
  }
  /**
   * @brief Get the end_date of object
   *
   * @return Date | undefined the endDate of object
   */
  public get endDate(): Date | undefined {
    return this._endDate;
  }
  /**
   * @brief Set the win of object
   *
   * @param value The new win value of object
   */
  public set win(value: boolean | undefined) {
    this._win = value;
  }
  /**
   * @brief Get the win of object
   *
   * @return boolean | undefined the win of object
   */
  public get win(): boolean | undefined {
    return this._win;
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
  /**
   * @brief Set the chat of object
   *
   * @param value The new chat value of object
   */
  public set chat(value: chat | undefined) {
    this._chat = value;
  }
  /**
   * @brief Get the chat of object
   *
   * @return chat | undefined the chat of object
   */
  public get chat(): chat | undefined {
    return this._chat;
  }
  /**
   * @brief Set the word of object
   *
   * @param value The new word value of object
   */
  public set word(value: word | undefined) {
    this._word = value;
  }
  /**
   * @brief Get the word of object
   *
   * @return word | undefined the word of object
   */
  public get word(): word | undefined {
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
