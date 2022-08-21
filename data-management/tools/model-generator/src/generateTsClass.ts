import { load } from "./loader";
import { ModelAttributesProperties, ModelClassDefinition } from "./modelTypes";
import { snakeCaseToCamelCase, snakeCaseToUpperCamelCase } from "./utils";

/**
 *
 * @param attrData
 * @returns
 */
function getTsAttributeType(attrData: { name: string; type: string }) {
  return attrData.type;
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
  set ${snakeCaseToCamelCase(attrData.name)}(value : ${getTsAttributeType(
    attrData
  )}) : void { this._${snakeCaseToCamelCase(attrData.name)} = value; };`;
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
   get ${snakeCaseToCamelCase(attrData.name)} () : ${getTsAttributeType(
    attrData
  )} { return this._${snakeCaseToCamelCase(attrData.name)}; };`;
}

/**
 *
 * @param attrData
 * @returns
 */
function generateTsAttribute(attrData: { name: string; type: string }) {
  return `
_${snakeCaseToCamelCase(attrData.name)}: ${getTsAttributeType(attrData)};`;
}

/**
 *
 * @param {*} modelClass
 */
export function generateTsClass(modelClass: ModelClassDefinition) {
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

  console.log(
    filename,
    load("TsClasses", {
      className,
      dependencies: "",
      extendedClasses: extendClass
        ? `extends ${snakeCaseToUpperCamelCase(extendClass)}`
        : "",
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
    })
  );
}
