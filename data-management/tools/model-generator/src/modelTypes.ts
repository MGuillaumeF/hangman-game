export type XMLModelClassDefinition = {
  $: XMLModelClassProperties;
  attributes: {
    attribute: {
      $: XMLModelAttributesProperties;
    }[];
  }[];
};

export type ModelClassDefinition = {
  abstract?: string;
  attributes: ModelAttributesProperties[];
  extend?: string;
  name: string;
};

export type XMLModelClassProperties = { extend?: string; name: string };

export type UnidirectionalCardinality = "to_one" | "to_many";

export type BidirectionalCardinality =
  | "one_to_one"
  | "one_to_many"
  | "many_to_many";

export type XMLModelAttributesProperties = {
  cardinality?: UnidirectionalCardinality | BidirectionalCardinality;
  default_value?: string;
  linked_column?: string;
  macro?: "increment-auto";
  mandatory?: string;
  max?: string;
  max_length?: string;
  min?: string;
  min_length?: string;
  name: string;
  object_id?: string;
  pattern?: string;
  type: string;
  visibility: "private" | "protected" | "public";
};

export type ModelAttributesProperties = {
  cardinality?: UnidirectionalCardinality | BidirectionalCardinality;
  default_value?: string;
  linked_column?: string;
  macro?: "increment-auto";
  mandatory?: boolean;
  max?: number;
  max_length?: number;
  min?: number;
  min_length?: number;
  name: string;
  object_id: boolean;
  pattern?: string;
  type: string;
  visibility: "private" | "protected" | "public";
};
