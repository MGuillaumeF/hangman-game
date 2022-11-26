/**
 * @filename User.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { RootModelObject } from "./RootModelObject";
import { Group } from "./Group";
import { Team } from "./Team";
import { Chat } from "./Chat";
import { Party } from "./Party";
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class User extends RootModelObject {
  protected static getConstraintes() {
    return {
      ...RootModelObject.getConstraintes(),
      ...{
        login: {
          mandatory: true,
          min_length: 3,
          pattern: "^[a-zA-Z0-9]{3,32}$",
          type: "string"
        },
        password: {
          mandatory: true,
          min_length: 8,
          pattern:
            "^.*(?:(?:[a-z].*[A-Z].*\\\\d)|(?:[A-Z].*\\\\d.*[a-z])|(?:\\\\d.*[a-z].*[A-Z])).*$",
          type: "string"
        },
        salt_user: {
          mandatory: true,
          min_length: 32,
          type: "string"
        },
        salt_session: {
          mandatory: true,
          min_length: 32,
          type: "string"
        },
        token: {
          mandatory: true,
          min_length: 32,
          type: "string"
        },
        last_connection: {
          mandatory: false,
          type: "Date"
        }
      }
    };
  }

  private _login: string | undefined;

  private _password: string | undefined;

  private _saltUser: string | undefined;

  private _saltSession: string | undefined;

  private _token: string | undefined;

  private _lastConnection: Date | undefined;

  private _friends: User[] | undefined;

  private _groups: Group[] | undefined;

  private _teams: Team[] | undefined;

  private _chats: Chat[] | undefined;

  private _parties: Party[] | undefined;

  /**
   * @brief Set the login of object
   *
   * @param value The new login value of object
   */
  public set login(value: string | undefined) {
    this._login = value;
  }
  /**
   * @brief Get the login of object
   *
   * @return string | undefined the login of object
   */
  public get login(): string | undefined {
    return this._login;
  }
  /**
   * @brief Set the password of object
   *
   * @param value The new password value of object
   */
  public set password(value: string | undefined) {
    this._password = value;
  }
  /**
   * @brief Get the password of object
   *
   * @return string | undefined the password of object
   */
  public get password(): string | undefined {
    return this._password;
  }
  /**
   * @brief Set the salt_user of object
   *
   * @param value The new salt_user value of object
   */
  public set saltUser(value: string | undefined) {
    this._saltUser = value;
  }
  /**
   * @brief Get the salt_user of object
   *
   * @return string | undefined the saltUser of object
   */
  public get saltUser(): string | undefined {
    return this._saltUser;
  }
  /**
   * @brief Set the salt_session of object
   *
   * @param value The new salt_session value of object
   */
  public set saltSession(value: string | undefined) {
    this._saltSession = value;
  }
  /**
   * @brief Get the salt_session of object
   *
   * @return string | undefined the saltSession of object
   */
  public get saltSession(): string | undefined {
    return this._saltSession;
  }
  /**
   * @brief Set the token of object
   *
   * @param value The new token value of object
   */
  public set token(value: string | undefined) {
    this._token = value;
  }
  /**
   * @brief Get the token of object
   *
   * @return string | undefined the token of object
   */
  public get token(): string | undefined {
    return this._token;
  }
  /**
   * @brief Set the last_connection of object
   *
   * @param value The new last_connection value of object
   */
  public set lastConnection(value: Date | undefined) {
    this._lastConnection = value;
  }
  /**
   * @brief Get the last_connection of object
   *
   * @return Date | undefined the lastConnection of object
   */
  public get lastConnection(): Date | undefined {
    return this._lastConnection;
  }
  /**
   * @brief Set the friends of object
   *
   * @param value The new friends value of object
   */
  public set friends(value: User[] | undefined) {
    this._friends = value;
  }
  /**
   * @brief Get the friends of object
   *
   * @return User[] | undefined the friends of object
   */
  public get friends(): User[] | undefined {
    return this._friends;
  }
  /**
   * @brief Set the groups of object
   *
   * @param value The new groups value of object
   */
  public set groups(value: Group[] | undefined) {
    this._groups = value;
  }
  /**
   * @brief Get the groups of object
   *
   * @return Group[] | undefined the groups of object
   */
  public get groups(): Group[] | undefined {
    return this._groups;
  }
  /**
   * @brief Set the teams of object
   *
   * @param value The new teams value of object
   */
  public set teams(value: Team[] | undefined) {
    this._teams = value;
  }
  /**
   * @brief Get the teams of object
   *
   * @return Team[] | undefined the teams of object
   */
  public get teams(): Team[] | undefined {
    return this._teams;
  }
  /**
   * @brief Set the chats of object
   *
   * @param value The new chats value of object
   */
  public set chats(value: Chat[] | undefined) {
    this._chats = value;
  }
  /**
   * @brief Get the chats of object
   *
   * @return Chat[] | undefined the chats of object
   */
  public get chats(): Chat[] | undefined {
    return this._chats;
  }
  /**
   * @brief Set the parties of object
   *
   * @param value The new parties value of object
   */
  public set parties(value: Party[] | undefined) {
    this._parties = value;
  }
  /**
   * @brief Get the parties of object
   *
   * @return Party[] | undefined the parties of object
   */
  public get parties(): Party[] | undefined {
    return this._parties;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      ...Validator.checkStringProperty(
        User.getConstraintes().login,
        "login",
        this.login
      )
    );
    errors.push(
      ...Validator.checkStringProperty(
        User.getConstraintes().password,
        "password",
        this.password
      )
    );
    errors.push(
      ...Validator.checkStringProperty(
        User.getConstraintes().salt_user,
        "saltUser",
        this.saltUser
      )
    );
    errors.push(
      ...Validator.checkStringProperty(
        User.getConstraintes().salt_session,
        "saltSession",
        this.saltSession
      )
    );
    errors.push(
      ...Validator.checkStringProperty(
        User.getConstraintes().token,
        "token",
        this.token
      )
    );
    if (this.friends !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.friends.map((item) => item.getErrors())
        )
      );
    }
    if (this.groups !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.groups.map((item) => item.getErrors())
        )
      );
    }
    if (this.teams !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.teams.map((item) => item.getErrors())
        )
      );
    }
    if (this.chats !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.chats.map((item) => item.getErrors())
        )
      );
    }
    if (this.parties !== undefined) {
      errors.push(
        ...([] as ModelError[]).concat(
          ...this.parties.map((item) => item.getErrors())
        )
      );
    }
    return errors;
  }
  /**
   * @brief method to convert object to JSON object
   */
  public toJSON(): any {
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
      chats,
      parties
    } = this;
    return {
      login,
      password,
      saltUser,
      saltSession,
      token,
      lastConnection,
      friends:
        friends !== undefined
          ? friends.map((item) => item.toJSON())
          : undefined,
      groups:
        groups !== undefined ? groups.map((item) => item.toJSON()) : undefined,
      teams:
        teams !== undefined ? teams.map((item) => item.toJSON()) : undefined,
      chats:
        chats !== undefined ? chats.map((item) => item.toJSON()) : undefined,
      parties:
        parties !== undefined ? parties.map((item) => item.toJSON()) : undefined
    };
  }

  public static parse(data: any): User {
    const obj = RootModelObject.parse<User>(data);
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
        if (data["last_connection"] instanceof Date) {
          obj.lastConnection = data["last_connection"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["friends"] !== undefined) {
        if (Array.isArray(data["friends"])) {
          obj.friends = data["friends"].map((item) => User.parse(item));
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["groups"] !== undefined) {
        if (Array.isArray(data["groups"])) {
          obj.groups = data["groups"].map((item) => Group.parse(item));
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["teams"] !== undefined) {
        if (Array.isArray(data["teams"])) {
          obj.teams = data["teams"].map((item) => Team.parse(item));
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["chats"] !== undefined) {
        if (Array.isArray(data["chats"])) {
          obj.chats = data["chats"].map((item) => Chat.parse(item));
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["parties"] !== undefined) {
        if (Array.isArray(data["parties"])) {
          obj.parties = data["parties"].map((item) => Party.parse(item));
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
