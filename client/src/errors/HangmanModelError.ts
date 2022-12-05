import { ModelError } from "../model/ModelError"

/**
 * Custom model error
 */
export default class HangmanModelError extends HangmanError {
  /**
   * error name to get it when errro is raised
   */
  public readonly name = HangmanModelError.name;

  /**
   * The list of model errors
   */
  private _modelErrors : ModelError[] = [];

  /**
   * @param message the message of error
   * @param options the cause or error
   */
  constructor(message: string, modelErrors : ModelError[] = [], options?: { cause: Error }) {
    // call parent constructor
    super(message, options);
    this._modelErrors = modelErrors;
  }

  /**
   * @brief Set the model error array of object
   *
   * @param value The new model error array value of object
   */
  public set modelErrors(value: ModelError[]) {
    this._modelErrors = value;
  }

  /**
   * @brief Get the model error array of object
   *
   * @return ModelError[] the model error array of object
   */
  public get modelErrors(): ModelError[] {
    return this._modelErrors;
  }

  /**
   * @brief Add errors in the model error array of object
   *
   * @param values The data list to add in model error array value of object
   */
  public modelErrors(...values: ModelError[]) {
    this._modelErrors.push(...values);
  }
}
