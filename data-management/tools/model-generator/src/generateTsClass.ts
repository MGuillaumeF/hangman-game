import { load } from "./loader";
import { ModelAttributesProperties, ModelClassDefinition } from "./modelTypes";
import { snakeCaseToCamelCase, snakeCaseToUpperCamelCase } from "./utils";

export class TypeScriptClassGenerator {
  private static classNames: Set<string>;
  private currentName: string;
  private dependencies: Set<string> = new Set<string>();
  constructor(currentClassName: string) {
    this.currentName = currentClassName;
  }

  public static set classNames(value :Set<string>) {this.classNames = value;}
  public generateAttributeType(
    attibuteProperties: ModelAttributesProperties
  ): string {
    if (TypeScriptClassGenerator.classNames.has(attibuteProperties.type)) {
      this.dependencies.add(snakeCaseToUpperCamelCase(attibuteProperties.type));
      return snakeCaseToUpperCamelCase(attibuteProperties.type);
    } else if (Object.keys(tsMapTypes).includes(attibuteProperties.type)) {
      return String(tsMapTypes[attibuteProperties.type]);
    } else {
      return attibuteProperties.type;
    }
  }
}

const tsMapTypes: { [key: string]: string } = {
  date: "Date",
  string: "string",
  int8: "number",
  int16: "number",
  int32: "number",
  int64: "number",
  uint8: "number",
  uint16: "number",
  uint32: "number",
  uint64: "number"
};

/**
 *
 * @param attrData
 * @returns
 */
function getTsAttributeType(attrData: { name: string; type: string }) {
  return Object.keys(tsMapTypes).includes(attrData.type)
    ? tsMapTypes[attrData.type]
    : attrData.type;
}

/**
 *
 * @param attrData
 * @returns
 */
function generateTsSetter(attrData: { name: string; type: string }) {
  return `
/**
 * @brief Set the ${attrData.name} of object
 *
 * @param value The new ${attrData.name} value of object
 */
  public set ${snakeCaseToCamelCase(
    attrData.name
  )}(value : ${getTsAttributeType(attrData)}) { this._${snakeCaseToCamelCase(
    attrData.name
  )} = value; };`;
}

/**
 *
 * @param attrData
 * @returns
 */
function generateTsGetter(attrData: { name: string; type: string }) {
  return `
  /**
   * @brief Get the ${attrData.name} of object
   *
   * @return ${getTsAttributeType(attrData)} the ${snakeCaseToCamelCase(
    attrData.name
  )} of object
   */
   public get ${snakeCaseToCamelCase(attrData.name)} () : ${getTsAttributeType(
    attrData
  )} { return this._${snakeCaseToCamelCase(attrData.name)}; };`;
}

/**
 *
 * @param attrData
 * @returns
 */
function generateTsAttribute(attrData: ModelAttributesProperties) {
  return `
${attrData.visibility} _${snakeCaseToCamelCase(
    attrData.name
  )}: ${getTsAttributeType(attrData)};`;
}

/**
 *
 * @param modelClass
 * @returns
 */
export function generateTsClass(modelClass: ModelClassDefinition): string {
  const className = snakeCaseToUpperCamelCase(modelClass.$.name);
  const extendClass = modelClass.$.extend;
  const filename = `${className}.ts`;

  const attributes: {
    $: ModelAttributesProperties;
  }[] = modelClass.attributes[0]?.attribute || [];
  const publicMethods: string[] = [];
  const privateAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.$.visibility === "private";
    })
    .map((attributeObject) => {
      const attrData = attributeObject.$;
      publicMethods.push(generateTsSetter(attrData));
      publicMethods.push(generateTsGetter(attrData));
      return generateTsAttribute(attrData);
    })
    .join("\n");

  const protectedAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.$.visibility === "protected";
    })
    .map((attributeObject) => {
      const attrData = attributeObject.$;
      publicMethods.push(generateTsSetter(attrData));
      publicMethods.push(generateTsGetter(attrData));
      return generateTsAttribute(attrData);
    })
    .join("\n");
  const generatedTsTemplate = load("TsClasses", {
    className,
    dependencies: "",
    extendedClasses: extendClass
      ? `extends ${snakeCaseToUpperCamelCase(extendClass)}`
      : "",
    filename,
    privateAttributes,
    protectedAttributes,
    privateStaticAttributes: "",
    privateMethods: "",
    privateStaticMethods: "",
    protectedStaticAttributes: "",
    protectedMethods: "",
    protectedStaticMethods: "",
    publicAttributes: "",
    publicStaticAttributes: "",
    publicMethods: publicMethods ? publicMethods.join("") : "",
    publicStaticMethods: ""
  });
  console.log(filename, generatedTsTemplate);
  return generatedTsTemplate;
}
