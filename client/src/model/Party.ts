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

export default class Party extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        start_date: {
          mandatory: true,
          type: "Date"
        },
        end_date: {
          mandatory: false,
          type: "Date"
        }
      }
    };
  }

  private _startDate: Date | undefined;

  private _endDate: Date | undefined;

  private _win: boolean | undefined;

  private _members: User[] | undefined;

  private _chat: Chat | undefined;

  private _word: Word | undefined;

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
  /**
   * @brief Set the word of object
   *
   * @param value The new word value of object
   */
  public set word(value: Word | undefined) {
    this._word = value;
  }
  /**
   * @brief Get the word of object
   *
   * @return Word | undefined the word of object
   */
  public get word(): Word | undefined {
    return this._word;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    if (this.members !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.members.map((item) => item.getErrors())
        )
      );
    }
    if (this.chat !== undefined) {
      errors.push(...this.chat.getErrors());
    }
    if (this.word !== undefined) {
      errors.push(...this.word.getErrors());
    }
    return errors;
  }
  /**
   * @brief method to convert object to JSON object
   */
  public toJSON(): any {
    const { startDate, endDate, win, members, chat, word } = this;
    return {
      startDate,
      endDate,
      win,
      members:
        members !== undefined
          ? members.map((item) => item.toJSON())
          : undefined,
      chat: chat !== undefined ? chat.toJSON() : undefined,
      word: word !== undefined ? word.toJSON() : undefined
    };
  }

  public static parse(data: any): Party {
    const obj = RootModelObject.parseMetaData<Party>(new Party(), data);
    if (typeof data === "object") {
      if (data["start_date"] !== undefined) {
        if (data["start_date"] instanceof Date) {
          obj.startDate = data["start_date"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["end_date"] !== undefined) {
        if (data["end_date"] instanceof Date) {
          obj.endDate = data["end_date"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["win"] !== undefined) {
        if (typeof data["win"] === "boolean") {
          obj.win = data["win"];
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
      if (data["chat"] !== undefined) {
        if (typeof data["chat"] === "object") {
          obj.chat = Chat.parse(data["chat"]);
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["word"] !== undefined) {
        if (typeof data["word"] === "object") {
          obj.word = Word.parse(data["word"]);
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
