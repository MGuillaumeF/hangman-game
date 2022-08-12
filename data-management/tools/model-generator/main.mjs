
import { parseString } from "xml2js";
import { readFileSync } from "fs";
import path from "path";
import {fileURLToPath} from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const cppMapTypes = {
  "date" : "std::time_t",
  "string" : "std::string",
  "int8" : "int8_t",
  "int16" : "int16_t",
  "int32" : "int32_t",
  "int64" : "int64_t",
  "uint8" : "uint8_t",
  "uint16" : "uint16_t",
  "uint32" : "uint32_t",
  "uint64" : "uint64_t"
}

const cppMapIncludes = {
  "date" : "ctime",
  "string" : "string"
}

function generateClasses(modelClasses) {
  for (const modelClass of modelClasses) {
    generateCppClass(modelClass);
    generateTsClass(modelClass);
  }
}

function generateCppClass(modelClass) {
  const className = modelClass.$.name
  const extendClass = modelClass.$.extend
  const filename = `${className}.hxx`;
  const guard = `__${className.toUpperCase()}_HXX__`;

  const includesCpp = new Set();
  const attributes = modelClass.attributes[0].attribute;
  const privateAttributes = attributes.filter(attributeObject => {
    return attributeObject.$.visibility == "private";
  }).map(attributeObject => {
    const attrData = attributeObject.$;
    if (cppMapIncludes[attrData.type]) {
      includesCpp.add(cppMapIncludes[attrData.type]);
    }
    return `${cppMapTypes[attrData.type] ? cppMapTypes[attrData.type] : attrData.type } m_${attrData.name};`
  }).join("\n");
  const protectedAttributes = attributes.filter(attributeObject => {
    return attributeObject.$.visibility == "protected";
  }).map(attributeObject => {
    const attrData = attributeObject.$;
    if (cppMapIncludes[attrData.type]) {
      includesCpp.add(cppMapIncludes[attrData.type]);
    }
    return `${cppMapTypes[attrData.type] ? cppMapTypes[attrData.type] : attrData.type } m_${attrData.name};`
  }).join("\n");
  const publicAttributes = attributes.filter(attributeObject => {
    return attributeObject.$.visibility == "public";
  }).map(attributeObject => {
    const attrData = attributeObject.$;
    if (cppMapIncludes[attrData.type]) {
      includesCpp.add(cppMapIncludes[attrData.type]);
    }
    return `${cppMapTypes[attrData.type] ? cppMapTypes[attrData.type] : attrData.type } m_${attrData.name};`
  }).join("\n");

  const cppClassTemplate = `#ifndef ${guard}
#define ${guard} 

${Array.from(includesCpp).map(inc => `#include <${inc}>`).join("\n")}

/**
 * @brief class of ${className} in model
 *
 */
#pragma db object
class ${className} ${extendClass ? `final : public ${extendClass}` : ''} {
private:
${privateAttributes.map(attributeObject => {
  const attrData = attributeObject.$;
  return `${attrData.type} m_${attrData.name};`
}).join("\n")}

protected:
${protectedAttributes.map(attributeObject => {
  const attrData = attributeObject.$;
  return `${attrData.type} m_${attrData.name};`
}).join("\n")}

public:
  /**
   * @brief Construct a new ${className} object
   *
   */
  ${className}() = default;

${publicAttributes.map(attributeObject => {
  const attrData = attributeObject.$;
  return `${attrData.type} m_${attrData.name};`
}).join("\n")}


};
#endid // end ${guard}`;

  console.info("generated :", cppClassTemplate);

}
function generateTsClass(modelClass) {

}



const xml = readFileSync(path.resolve(__dirname, "../../resources/model.xml"));
parseString(xml, function (err, result) {
    console.info("xml parsing result", JSON.stringify(result, null, 1));
    generateClasses(result.model.classes[0].class);
});

