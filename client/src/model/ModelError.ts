export enum EAttributeError {
  MAX_LENGTH = "MAX_LENGTH",
  MAX_SIZE = "MAX_SIZE",
  MIN_LENGTH = "MIN_LENGTH",
  MIN_SIZE = "MIN_SIZE",
  MUST_BE_NULL = "MUST_BE_NULL",
  MUST_NOT_NULL = "MUST_NOT_NULL",
  PATTERN = "PATTERN"
}

export type ModelError = {
  attributeName: string;
  code: EAttributeError;
  message?: string;
};
