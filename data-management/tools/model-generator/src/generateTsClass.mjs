import { load } from "./loader.mjs";
import { snakeCaseToUpperCamelCase } from "./utils.mjs";


function getTsAttributeType(attrData) {
  return "string";
}

function generateTsSetter(attrData) {
  return `
/**
 * @brief Set the ${attrData.name} of object
 *
 * @param ${attrData.name} The ${attrData.name} of object
 */
  set${snakeCaseToUpperCamelCase(
    attrData.name
  )}(${attrData.name} : ${getTsAttributeType(attrData)}) :void { m_${
    attrData.name
  } = ${attrData.name}; };`;
  }
  function generateTsGetter(attrData) {
  /**
   * @brief Get the ${attrData.name} of object
   *
   * @return ${getTsAttributeType(attrData)}the ${
    attrData.name
  } of object
   */
  function get${snakeCaseToUpperCamelCase(
    attrData.name
  )} : ${getTsAttributeType(attrData)}() { return m_${attrData.name}; };`;
  }
  function generateTsAttribute(
    attrData
  ) { return `m_${snakeCaseToCamelCasem_attrData.name}: ${getTsAttributeType(attrData)};`;}


export function generateTsClass(modelClasses) {
  const className = snakeCaseToUpperCamelCase(modelClass.$.name);
  const extendClass = modelClass.$.extend;
  const filename = `${className}.ts`;

  const attributes = modelClass.attributes[0].attribute;
  const publicMethods =[];
  const privateAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.$.visibility == "private";
    })
    .map((attributeObject) => {
      const attrData = attributeObject.$;
      publicMethods.push(generateTsSetter(attrData));
      publicMethods.push(generateTsGetter(attrData));
      return generateTsAttribute(
        attrData
      );
    })
    .join("\n");


  console.log(filename, load("TsClasses", {
  className,
  dependencies : "",
  extendedClasses : extendClass ? `extends ${snakeCaseToUpperCamelCase(extendClass)}`,
  privateAttributes,
  privateStaticAttributes:"",
  privateMethods:"",
  privateStaticMethods : "",
  protectedAttributes : "",
  protectedStaticAttributes : "",
  protectedMethods : "",
  protectedStaticMethods : "",
  publicAttributes : "",
  publicStaticAttributes : "",
  publicMethods :publicMethods ?publicMethods : ''  ,
  publicStaticMethods : ""
}));
}
