import { ModelError } from "ModelError";

export class Validator {
  public static checkStringProperty(
    constraint: {
      mandatory: boolean;
      min_length?: number;
      max_length?: number;
      pattern?: RegExp;
    },
    value?: string
  ): ModelError[] {
    return [];
  }

  public static checkStringProperty(
    constraint: {
      mandatory: boolean;
      min?: number;
      max?: number;
    },
    value?: number
  ): ModelError[] {
    return [];
  }
}
