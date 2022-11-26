/**
 * @filename RootModelObject.ts
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */
import { Validator } from "./Validator";
import { ModelError } from "./ModelError";

export abstract class RootModelObject {
  protected static getConstraintes() {
    return {
      version: {
        mandatory: true,
        min: 1,
        type: "number"
      },
      id: {
        mandatory: true,
        min: 1,
        type: "number"
      },
      created_by: {
        mandatory: true,
        min_length: 3,
        type: "string"
      },
      created_at: {
        mandatory: true,
        type: "Date"
      },
      updated_by: {
        mandatory: true,
        min_length: 3,
        type: "string"
      },
      updated_at: {
        mandatory: true,
        type: "Date"
      }
    };
  }

  protected _version: number | undefined;

  protected _id: number | undefined;

  protected _createdBy: string | undefined;

  protected _createdAt: Date | undefined;

  protected _updatedBy: string | undefined;

  protected _updatedAt: Date | undefined;

  /**
   * @brief Set the version of object
   *
   * @param value The new version value of object
   */
  public set version(value: number | undefined) {
    this._version = value;
  }
  /**
   * @brief Get the version of object
   *
   * @return number | undefined the version of object
   */
  public get version(): number | undefined {
    return this._version;
  }
  /**
   * @brief Set the id of object
   *
   * @param value The new id value of object
   */
  public set id(value: number | undefined) {
    this._id = value;
  }
  /**
   * @brief Get the id of object
   *
   * @return number | undefined the id of object
   */
  public get id(): number | undefined {
    return this._id;
  }
  /**
   * @brief Set the created_by of object
   *
   * @param value The new created_by value of object
   */
  public set createdBy(value: string | undefined) {
    this._createdBy = value;
  }
  /**
   * @brief Get the created_by of object
   *
   * @return string | undefined the createdBy of object
   */
  public get createdBy(): string | undefined {
    return this._createdBy;
  }
  /**
   * @brief Set the created_at of object
   *
   * @param value The new created_at value of object
   */
  public set createdAt(value: Date | undefined) {
    this._createdAt = value;
  }
  /**
   * @brief Get the created_at of object
   *
   * @return Date | undefined the createdAt of object
   */
  public get createdAt(): Date | undefined {
    return this._createdAt;
  }
  /**
   * @brief Set the updated_by of object
   *
   * @param value The new updated_by value of object
   */
  public set updatedBy(value: string | undefined) {
    this._updatedBy = value;
  }
  /**
   * @brief Get the updated_by of object
   *
   * @return string | undefined the updatedBy of object
   */
  public get updatedBy(): string | undefined {
    return this._updatedBy;
  }
  /**
   * @brief Set the updated_at of object
   *
   * @param value The new updated_at value of object
   */
  public set updatedAt(value: Date | undefined) {
    this._updatedAt = value;
  }
  /**
   * @brief Get the updated_at of object
   *
   * @return Date | undefined the updatedAt of object
   */
  public get updatedAt(): Date | undefined {
    return this._updatedAt;
  }
  public getErrors(): ModelError[] {
    const errors: ModelError[] = [];
    errors.push(
      ...Validator.checkNumberProperty(
        RootModelObject.getConstraintes().version,
        "version",
        this.version
      )
    );
    errors.push(
      ...Validator.checkNumberProperty(
        RootModelObject.getConstraintes().id,
        "id",
        this.id
      )
    );
    errors.push(
      ...Validator.checkStringProperty(
        RootModelObject.getConstraintes().created_by,
        "createdBy",
        this.createdBy
      )
    );
    errors.push(
      ...Validator.checkStringProperty(
        RootModelObject.getConstraintes().updated_by,
        "updatedBy",
        this.updatedBy
      )
    );
    return errors;
  }
  /**
   * @brief method to convert object to JSON object
   */
  public toJSON(): any {
    const { version, id, createdBy, createdAt, updatedBy, updatedAt } = this;
    return { version, id, createdBy, createdAt, updatedBy, updatedAt };
  }

  public static parseMetadata<T extends RootModelObject>(data: any): T {
    const obj = new T();
    if (typeof data === "object") {
      if (data["version"] !== undefined) {
        if (typeof data["version"] === "number") {
          obj.version = data["version"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
      if (data["id"] !== undefined) {
        if (typeof data["id"] === "number") {
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
        if (data["created_at"] instanceof Date) {
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
        if (data["updated_at"] instanceof Date) {
          obj.updatedAt = data["updated_at"];
        } else {
          throw Error("INVALID TYPE");
        }
      }
    }
    return obj;
  }
}
