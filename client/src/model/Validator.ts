import { EAttributeError, ModelError } from "./ModelError";

export class Validator {
  public static checkStringProperty(
    constraint: {
      mandatory: boolean;
      max_length?: number;
      min_length?: number;
      pattern?: RegExp;
    },
    attributeName: string,
    value?: string
  ): ModelError[] {
    const errors: ModelError[] = [];
    if (value === undefined && constraint.mandatory) {
      errors.push({ attributeName, code: EAttributeError.MUST_NOT_NULL });
    }
    if (
      value !== undefined &&
      constraint.max_length !== undefined &&
      value.length > constraint.max_length
    ) {
      errors.push({ attributeName, code: EAttributeError.MAX_LENGTH });
    }
    if (
      value !== undefined &&
      constraint.min_length !== undefined &&
      value.length < constraint.min_length
    ) {
      errors.push({ attributeName, code: EAttributeError.MAX_LENGTH });
    }
    if (
      value !== undefined &&
      constraint.pattern !== undefined &&
      !new RegExp(constraint.pattern).test(value)
    ) {
      errors.push({ attributeName, code: EAttributeError.PATTERN });
    }
    return errors;
  }

  public static checkNumberProperty(
    constraint: {
      mandatory: boolean;
      max?: number;
      min?: number;
    },
    attributeName: string,
    value?: number
  ): ModelError[] {
    const errors: ModelError[] = [];
    if (value === undefined && constraint.mandatory) {
      errors.push({ attributeName, code: EAttributeError.MUST_NOT_NULL });
    }
    if (
      value !== undefined &&
      constraint.max !== undefined &&
      value > constraint.max
    ) {
      errors.push({ attributeName, code: EAttributeError.MAX_SIZE });
    }
    if (
      value !== undefined &&
      constraint.min !== undefined &&
      value < constraint.min
    ) {
      errors.push({ attributeName, code: EAttributeError.MIN_SIZE });
    }
    return errors;
  }
}
