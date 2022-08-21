export type ModelClassDefinition = {
  $: ModelClassProperties;
  attributes: {
    attribute: {
      $: ModelAttributesProperties;
    }[];
  }[];
};

export type ModelClassProperties = { extend?: string; name: string };

export type UnidirectionalCardinality = "to_one" | "to_many";

export type BidirectionalCardinality =
  | "one_to_one"
  | "one_to_many"
  | "many_to_many";

export type ModelAttributesProperties = {
  cardinality?: UnidirectionalCardinality | BidirectionalCardinality;
  linked_column?: string;
  name: string;
  type: string;
  visibility: "private" | "protected" | "public";
};
