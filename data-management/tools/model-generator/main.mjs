import { parseString } from "xml2js";
import { readFileSync } from "fs";
import path from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const cppMapTypes = {
  date: "std::time_t",
  string: "std::string",
  int8: "int8_t",
  int16: "int16_t",
  int32: "int32_t",
  int64: "int64_t",
  uint8: "uint8_t",
  uint16: "uint16_t",
  uint32: "uint32_t",
  uint64: "uint64_t",
};

const allClassNames = new Set();

const cppMapIncludes = {
  date: "ctime",
  string: "string",
};

function toCapitalize(str) {
  return `${str.charAt(0).toUpperCase()}${str.slice(1)}`;
}

function snakeCaseToCamelCase(str) {
  const words = str.split("_");
  return `${words[0]}${words.slice(1).map(toCapitalize).join("")}`;
}

function snakeCaseToUpperCamelCase(str) {
  const words = str.split("_");
  return words.map(toCapitalize).join("");
}

function getCppAttributeType(attrData, includesLib, includesObjects) {
  let attributeType = attrData.type;
  const isArray = /^.*\[\]$/.test(attrData.type)
  if (isArray(attributeType)) {
    attributeType = attributeType.slice(0, -2);
    includesLib.add("vector");
  }
  if (cppMapTypes[attributeType]) {
    attributeType = cppMapTypes[attributeType];
    includesLib.add(attributeType);
  } else if (allClassNames.contains(attributeType)) {
    includesObjects.add(attributeType);
  }
  return isArray ? `std::vector<${attributeType}>` : attributeType;
}

function generateClasses(modelClasses) {
  for (const modelClass of modelClasses) {
    generateCppClass(modelClass);
    generateTsClass(modelClass);
  }
}

function generateCppSetter(attrData) {
  return `
  /**
   * @brief Set the ${attrData.name} of object
   *
   * @param ${attrData.name} The ${attrData.name} of object
   */
  void set${snakeCaseToUpperCamelCase(attrData.name)}(const ${
    cppMapTypes[attrData.type] ? cppMapTypes[attrData.type] : attrData.type
  } &${attrData.name}) { m_${attrData.name} = ${attrData.name}; };`;
}

function generateCppGetter(attrData) {
  return `
  /**
   * @brief Get the ${attrData.name} of object
   *
   * @return const ${
     cppMapTypes[attrData.type] ? cppMapTypes[attrData.type] : attrData.type
   }& the ${attrData.name} of object
   */
  const ${
    cppMapTypes[attrData.type] ? cppMapTypes[attrData.type] : attrData.type
  } &get${snakeCaseToUpperCamelCase(attrData.name)}() const { return m_${
    attrData.name
  }; };`;
}

function generateCppClass(modelClass) {
  const className = modelClass.$.name;
  const extendClass = modelClass.$.extend;
  const filename = `${className}.hxx`;
  const guard = `__${className.toUpperCase()}_HXX__`;

  const assessors = [];

  const includesCpp = new Set(["string"]);
  const includesModelObjectsCpp = new Set();
  const attributes = modelClass.attributes[0].attribute;

if (extendClass) {
    includesModelObjectsCpp.add(extendClass);
  }

  const privateAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.$.visibility == "private";
    })
    .map((attributeObject) => {
      const attrData = attributeObject.$;
      assessors.push(generateCppSetter(attrData));
      assessors.push(generateCppGetter(attrData));
      return `${getCppAttributeType(attrData, includesCpp, includesModelObjectsCpp)} m_${attrData.name};`;
    })
    .join("\n");
  const protectedAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.$.visibility == "protected";
    })
    .map((attributeObject) => {
      const attrData = attributeObject.$;
      assessors.push(generateCppSetter(attrData));
      assessors.push(generateCppGetter(attrData));
      return `${getCppAttributeType(attrData, includesCpp, includesModelObjectsCpp)} m_${attrData.name};`;
    
    })
    .join("\n");
  const publicAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.$.visibility == "public";
    })
    .map((attributeObject) => {
      const attrData = attributeObject.$;
      assessors.push(generateCppSetter(attrData));
      assessors.push(generateCppGetter(attrData));
      return `${getCppAttributeType(attrData, includesCpp, includesModelObjectsCpp)} m_${attrData.name};`;
    
    })
    .join("\n");

 

  const cppClassTemplate = `
/**
 * @filename ${filename}
 * @brief DO NOT MODIFY THIS FILE, this file is a generated model class
 */

#ifndef ${guard}
#define ${guard} 

${Array.from(includesModelObjectsCpp)
  .map((inc) => `#include "./${inc}.hxx"`)
  .join("\n")}

${Array.from(includesCpp)
  .map((inc) => `#include <${inc}>`)
  .join("\n")}

/**
 * @brief class of ${className} object in model
 *
 */
#pragma db object
class ${className} ${extendClass ? `final : public ${extendClass}` : ""} {
private:
${privateAttributes}

protected:
${protectedAttributes}

public:
  /**
   * @brief Construct a new ${className} object
   *
   */
  ${className}() = default;

${publicAttributes}

${assessors.join("\n\n")}

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "${className}"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "${
    className.slice(-1).toLowerCase() === "y"
      ? `${className.slice(0, -1)}ies`
      : `${className}s`
  }"; }

};
#endid // end ${guard}`;

  console.info("generated :", cppClassTemplate);
}
function generateTsClass(modelClass) {
  // TODO add template implementation of typescript classes
}

const xml = readFileSync(path.resolve(__dirname, "../../resources/model.xml"));
parseString(xml, function (err, result) {
  console.info("xml parsing result", JSON.stringify(result, null, 1));
  for (const modelClass of result.model.classes[0].class) {
    allClassNames.add(modelClass.$.name);
  }
  generateClasses(result.model.classes[0].class);
});
