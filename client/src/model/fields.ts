export type Field = {
  description: string;
  in: string;
  name: string;
  required: boolean;
  type: string;
};

export const isValidField = (
  name: string,
  field: Field | undefined
): field is Field => {
  if (field === undefined) {
    throw Error(`Invalid values for field ${name}, object is undefined`);
  }
  if (field.name === undefined || name !== field.name) {
    throw Error(
      `Invalid values for field ${name}, object name found is ${field.name}`
    );
  }
  return true;
};
