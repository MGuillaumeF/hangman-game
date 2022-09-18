export enum HttpMethod {
  DELETE = "DELETE",
  GET = "GET",
  PATCH = "PATCH",
  POST = "POST",
  PUT = "PUT"
}

export interface Field {
  id: string;
  name: string;
  required: boolean;
  value?: string | boolean | number;
}

export type FormUnit = string | boolean | number | null;
export type FormUnitArray = string[] | boolean[] | number[] | null[];
export type FormMultiUnit = FormUnit | FormUnitArray;
export type FormIndexKey = string | number;
export type FormAggregateObject = {
  [key: FormIndexKey]: FormMultiUnit | FormAggregateObject;
};
export type FormAggregateArray = Array<FormMultiUnit | FormAggregateObject>;
