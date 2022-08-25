/**
 * @filename User.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { Group } from "./Group";
import { Team } from "./Team";
import { Chat } from "./Chat";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class User extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        login: {
          mandatory: "true",
          min_length: "3",
          pattern: "/^[a-zA-Z0-9]{3,32}/$",
          type: "string"
        },
        password: {
          mandatory: "true",
          min_length: "8",
          pattern:
            "/^.*(?:(?:[a-z].*[A-Z].*\\d)|(?:[A-Z].*\\d.*[a-z])|(?:\\d.*[a-z].*[A-Z])).*/$",
          type: "string"
        },
        salt_user: {
          mandatory: "true",
          min_length: "32",
          type: "string"
        },
        salt_session: {
          mandatory: "true",
          min_length: "32",
          type: "string"
        },
        token: {
          mandatory: "true",
          min_length: "32",
          type: "string"
        },
        last_connection: {
          mandatory: "false",
          type: "Date"
        }
      }
    };
  }

  private _login: string;

  private _password: string;

  private _saltUser: string;

  private _saltSession: string;

  private _token: string;

  private _lastConnection: Date;

  private _friends: User[];

  private _groups: Group[];

  private _teams: Team[];

  private _chats: Chat[];

  /**
   * @brief Set the login of object
   *
   * @param value The new login value of object
   */
  public set login(value: string) {
    this._login = value;
  }
  /**
   * @brief Get the login of object
   *
   * @return string the login of object
   */
  public get login(): string {
    return this._login;
  }
  /**
   * @brief Set the password of object
   *
   * @param value The new password value of object
   */
  public set password(value: string) {
    this._password = value;
  }
  /**
   * @brief Get the password of object
   *
   * @return string the password of object
   */
  public get password(): string {
    return this._password;
  }
  /**
   * @brief Set the salt_user of object
   *
   * @param value The new salt_user value of object
   */
  public set saltUser(value: string) {
    this._saltUser = value;
  }
  /**
   * @brief Get the salt_user of object
   *
   * @return string the saltUser of object
   */
  public get saltUser(): string {
    return this._saltUser;
  }
  /**
   * @brief Set the salt_session of object
   *
   * @param value The new salt_session value of object
   */
  public set saltSession(value: string) {
    this._saltSession = value;
  }
  /**
   * @brief Get the salt_session of object
   *
   * @return string the saltSession of object
   */
  public get saltSession(): string {
    return this._saltSession;
  }
  /**
   * @brief Set the token of object
   *
   * @param value The new token value of object
   */
  public set token(value: string) {
    this._token = value;
  }
  /**
   * @brief Get the token of object
   *
   * @return string the token of object
   */
  public get token(): string {
    return this._token;
  }
  /**
   * @brief Set the last_connection of object
   *
   * @param value The new last_connection value of object
   */
  public set lastConnection(value: Date) {
    this._lastConnection = value;
  }
  /**
   * @brief Get the last_connection of object
   *
   * @return Date the lastConnection of object
   */
  public get lastConnection(): Date {
    return this._lastConnection;
  }
  /**
   * @brief Set the friends of object
   *
   * @param value The new friends value of object
   */
  public set friends(value: User[]) {
    this._friends = value;
  }
  /**
   * @brief Get the friends of object
   *
   * @return User[] the friends of object
   */
  public get friends(): User[] {
    return this._friends;
  }
  /**
   * @brief Set the groups of object
   *
   * @param value The new groups value of object
   */
  public set groups(value: Group[]) {
    this._groups = value;
  }
  /**
   * @brief Get the groups of object
   *
   * @return Group[] the groups of object
   */
  public get groups(): Group[] {
    return this._groups;
  }
  /**
   * @brief Set the teams of object
   *
   * @param value The new teams value of object
   */
  public set teams(value: Team[]) {
    this._teams = value;
  }
  /**
   * @brief Get the teams of object
   *
   * @return Team[] the teams of object
   */
  public get teams(): Team[] {
    return this._teams;
  }
  /**
   * @brief Set the chats of object
   *
   * @param value The new chats value of object
   */
  public set chats(value: Chat[]) {
    this._chats = value;
  }
  /**
   * @brief Get the chats of object
   *
   * @return Chat[] the chats of object
   */
  public get chats(): Chat[] {
    return this._chats;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      Validator.checkStringProperty(User.getConstraintes().login, this.login)
    );
    errors.push(
      Validator.checkStringProperty(
        User.getConstraintes().password,
        this.password
      )
    );
    errors.push(
      Validator.checkStringProperty(
        User.getConstraintes().salt_user,
        this.saltUser
      )
    );
    errors.push(
      Validator.checkStringProperty(
        User.getConstraintes().salt_session,
        this.saltSession
      )
    );
    errors.push(
      Validator.checkStringProperty(User.getConstraintes().token, this.token)
    );
    return errors;
  }
  public toJSON() {
    const {
      login,
      password,
      saltUser,
      saltSession,
      token,
      lastConnection,
      friends,
      groups,
      teams,
      chats
    } = this;
    return JSON.stringify({
      login,
      password,
      saltUser,
      saltSession,
      token,
      lastConnection,
      friends,
      groups,
      teams,
      chats
    });
  }

  public static parse(data: any): User {
    const obj = new User();
    if (typeof data === "object") {
      if (data["login"] !== undefined) {
        if (typeof data["login"] === "string") {
          obj.login = data["login"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["password"] !== undefined) {
        if (typeof data["password"] === "string") {
          obj.password = data["password"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["salt_user"] !== undefined) {
        if (typeof data["salt_user"] === "string") {
          obj.saltUser = data["salt_user"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["salt_session"] !== undefined) {
        if (typeof data["salt_session"] === "string") {
          obj.saltSession = data["salt_session"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["token"] !== undefined) {
        if (typeof data["token"] === "string") {
          obj.token = data["token"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["last_connection"] !== undefined) {
        if (typeof data["last_connection"] === "date") {
          obj.lastConnection = data["last_connection"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["friends"] !== undefined) {
        if (typeof data["friends"] === "user[]") {
          obj.friends = data["friends"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["groups"] !== undefined) {
        if (typeof data["groups"] === "group[]") {
          obj.groups = data["groups"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["teams"] !== undefined) {
        if (typeof data["teams"] === "team[]") {
          obj.teams = data["teams"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["chats"] !== undefined) {
        if (typeof data["chats"] === "chat[]") {
          obj.chats = data["chats"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
