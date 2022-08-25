/**
 * @filename RootModelObject.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export class RootModelObject {
  protected static getConstraintes() {
    return {
      version: {
        mandatory: "true",
        min: "1",
        type: "number"
      },
      id: {
        mandatory: "true",
        min: "1",
        type: "number"
      },
      created_by: {
        mandatory: "true",
        min_length: "3",
        type: "string"
      },
      created_at: {
        mandatory: "true",
        type: "Date"
      },
      updated_by: {
        mandatory: "true",
        min_length: "3",
        type: "string"
      },
      updated_at: {
        mandatory: "true",
        type: "Date"
      }
    };
  }

  protected _version: number;

  protected _id: number;

  protected _createdBy: string;

  protected _createdAt: Date;

  protected _updatedBy: string;

  protected _updatedAt: Date;

  /**
   * @brief Set the version of object
   *
   * @param value The new version value of object
   */
  public set version(value: number) {
    this._version = value;
  }
  /**
   * @brief Get the version of object
   *
   * @return number the version of object
   */
  public get version(): number {
    return this._version;
  }
  /**
   * @brief Set the id of object
   *
   * @param value The new id value of object
   */
  public set id(value: number) {
    this._id = value;
  }
  /**
   * @brief Get the id of object
   *
   * @return number the id of object
   */
  public get id(): number {
    return this._id;
  }
  /**
   * @brief Set the created_by of object
   *
   * @param value The new created_by value of object
   */
  public set createdBy(value: string) {
    this._createdBy = value;
  }
  /**
   * @brief Get the created_by of object
   *
   * @return string the createdBy of object
   */
  public get createdBy(): string {
    return this._createdBy;
  }
  /**
   * @brief Set the created_at of object
   *
   * @param value The new created_at value of object
   */
  public set createdAt(value: Date) {
    this._createdAt = value;
  }
  /**
   * @brief Get the created_at of object
   *
   * @return Date the createdAt of object
   */
  public get createdAt(): Date {
    return this._createdAt;
  }
  /**
   * @brief Set the updated_by of object
   *
   * @param value The new updated_by value of object
   */
  public set updatedBy(value: string) {
    this._updatedBy = value;
  }
  /**
   * @brief Get the updated_by of object
   *
   * @return string the updatedBy of object
   */
  public get updatedBy(): string {
    return this._updatedBy;
  }
  /**
   * @brief Set the updated_at of object
   *
   * @param value The new updated_at value of object
   */
  public set updatedAt(value: Date) {
    this._updatedAt = value;
  }
  /**
   * @brief Get the updated_at of object
   *
   * @return Date the updatedAt of object
   */
  public get updatedAt(): Date {
    return this._updatedAt;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      Validator.checkNumberProperty(
        RootModelObject.getConstraintes().version,
        this.version
      )
    );
    errors.push(
      Validator.checkNumberProperty(
        RootModelObject.getConstraintes().id,
        this.id
      )
    );
    errors.push(
      Validator.checkStringProperty(
        RootModelObject.getConstraintes().created_by,
        this.createdBy
      )
    );
    errors.push(
      Validator.checkStringProperty(
        RootModelObject.getConstraintes().updated_by,
        this.updatedBy
      )
    );
    return errors;
  }
  public toJSON() {
    const { version, id, createdBy, createdAt, updatedBy, updatedAt } = this;
    return JSON.stringify({
      version,
      id,
      createdBy,
      createdAt,
      updatedBy,
      updatedAt
    });
  }

  public static parse(data: any): RootModelObject {
    const obj = new RootModelObject();
    if (typeof data === "object") {
      if (data["version"] !== undefined) {
        if (typeof data["version"] === "uint32") {
          obj.version = data["version"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["id"] !== undefined) {
        if (typeof data["id"] === "uint32") {
          obj.id = data["id"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["created_by"] !== undefined) {
        if (typeof data["created_by"] === "string") {
          obj.createdBy = data["created_by"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["created_at"] !== undefined) {
        if (typeof data["created_at"] === "date") {
          obj.createdAt = data["created_at"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["updated_by"] !== undefined) {
        if (typeof data["updated_by"] === "string") {
          obj.updatedBy = data["updated_by"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["updated_at"] !== undefined) {
        if (typeof data["updated_at"] === "date") {
          obj.updatedAt = data["updated_at"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
