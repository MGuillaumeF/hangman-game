import { load } from "./loader";
import { ModelAttributesProperties, ModelClassDefinition } from "./modelTypes";
import { snakeCaseToCamelCase, snakeCaseToUpperCamelCase } from "./utils";

export class TypeScriptClassGenerator {
  private static _classNames: Set<string> = new Set<string>();
  private _currentName: string;
  private _dependencies: Set<string> = new Set<string>();

  constructor(currentClassName: string) {
    this._currentName = currentClassName;
  }

  public static set classNames(value: Set<string>) {
    TypeScriptClassGenerator._classNames = value;
  }

  public generateDependencies() {
    return Array.from(this._dependencies)
      .map((dependence) => {
        return `import {${dependence}} from "./${dependence}";`;
      })
      .join("\n");
  }

  public generateAttributeType(
    attibuteProperties: ModelAttributesProperties
  ): string {
    const isArray = /^.+\[\]$/.test(attibuteProperties.type);
    let typeObjectName = isArray
      ? attibuteProperties.type.slice(0, -2)
      : attibuteProperties.type;
    if (TypeScriptClassGenerator._classNames.has(typeObjectName)) {
      typeObjectName = snakeCaseToUpperCamelCase(typeObjectName);
      if (typeObjectName !== this._currentName) {
        this._dependencies.add(typeObjectName);
      }
    } else if (Object.keys(tsMapTypes).includes(typeObjectName)) {
      typeObjectName = String(tsMapTypes[typeObjectName]);
    }
    return isArray ? `${typeObjectName}[]` : typeObjectName;
  }

  /**
   *
   * @param attrData
   * @returns
   */
  public generateTsSetter(attrData: ModelAttributesProperties) {
    return `
/**
 * @brief Set the ${attrData.name} of object
 *
 * @param value The new ${attrData.name} value of object
 */
  public set ${snakeCaseToCamelCase(
    attrData.name
  )}(value : ${this.generateAttributeType(
      attrData
    )}) { this._${snakeCaseToCamelCase(attrData.name)} = value; };`;
  }

  /**
   *
   * @param attrData
   * @returns
   */
  public generateTsGetter(attrData: ModelAttributesProperties) {
    return `
  /**
   * @brief Get the ${attrData.name} of object
   *
   * @return ${this.generateAttributeType(attrData)} the ${snakeCaseToCamelCase(
      attrData.name
    )} of object
   */
   public get ${snakeCaseToCamelCase(
     attrData.name
   )} () : ${this.generateAttributeType(
      attrData
    )} { return this._${snakeCaseToCamelCase(attrData.name)}; };`;
  }

  /**
   *
   * @param attrData
   * @returns
   */
  public generateTsAttribute(attrData: ModelAttributesProperties) {
    return `
${attrData.visibility} _${snakeCaseToCamelCase(
      attrData.name
    )}: ${this.generateAttributeType(attrData)};`;
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
 * @param modelClass
 * @returns
 */
export function generateTsClass(modelClass: ModelClassDefinition): string {
  const className = snakeCaseToUpperCamelCase(modelClass.$.name);
  const generator = new TypeScriptClassGenerator(className);
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
      publicMethods.push(generator.generateTsSetter(attrData));
      publicMethods.push(generator.generateTsGetter(attrData));
      return generator.generateTsAttribute(attrData);
    })
    .join("\n");

  const protectedAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.$.visibility === "protected";
    })
    .map((attributeObject) => {
      const attrData = attributeObject.$;
      publicMethods.push(generator.generateTsSetter(attrData));
      publicMethods.push(generator.generateTsGetter(attrData));
      return generator.generateTsAttribute(attrData);
    })
    .join("\n");
  const generatedTsTemplate = load("TsClasses", {
    className,
    dependencies: generator.generateDependencies(),
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
