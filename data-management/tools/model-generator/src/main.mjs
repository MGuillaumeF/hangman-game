import { parseString } from "xml2js";
import { readFileSync } from "fs";
import path from "path";
import { fileURLToPath } from "url";
import { generateCRUDOrderDispatcher } from "./generateCRUDOrderDispatcher.mjs";

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
  const isArray = /^.*\[\]$/.test(attributeType);
  if (isArray) {
    attributeType = attributeType.slice(0, -2);
    if (includesLib) {
      includesLib.add("vector");
    }
  }

  if (includesLib && cppMapIncludes[attributeType]) {
    includesLib.add(cppMapIncludes[attributeType]);
  }

  if (cppMapTypes[attributeType]) {
    attributeType = cppMapTypes[attributeType];
  } else if (includesObjects && allClassNames.has(attributeType)) {
    includesObjects.add(attributeType);
  }
  if (attrData.cardinality && allClassNames.has(attributeType)) {
    if (includesLib) {
      includesLib.add("memory");
    }
    attributeType = `std::${
      attrData.linked_column ? "weak" : "shared"
    }_ptr<${attributeType}>`;
  }
  return isArray ? `std::vector<${attributeType}>` : attributeType;
}

function generateClasses(modelClasses) {
  for (const modelClass of modelClasses) {
    generateCppClass(modelClass);
    generateTsClass(modelClass);
  }
}

function generateCppPragma(attrData) {
  const pragmas = [];
  if (
    attrData.cardinality === "to_many" ||
    (attrData.cardinality === "many_to_many" && !attrData.linked_column)
  ) {
    pragmas.push("value_not_null", "unordered");
  } else if (
    (attrData.cardinality === "many_to_many" ||
      attrData.cardinality === "one_to_many") &&
    attrData.linked_column
  ) {
    pragmas.push("value_not_null", `inverse(m_${attrData.linked_column})`);
  } else if (
    attrData.cardinality === "to_one" ||
    (attrData.cardinality === "one_to_many" && !attrData.linked_column) ||
    (attrData.cardinality === "one_to_one" && !attrData.linked_column)
  ) {
    pragmas.push("not_null");
  } else if (attrData.cardinality === "one_to_one" && attrData.linked_column) {
    pragmas.push(`inverse(m_${attrData.linked_column})`);
  }
  return pragmas.length > 0
    ? ["#pragma", "db", ...pragmas, "\n"].join(" ")
    : "";
}

function generateCppAttribute(attrData, includesCpp, includesModelObjectsCpp) {
  return `${generateCppPragma(attrData)}${getCppAttributeType(
    attrData,
    includesCpp,
    includesModelObjectsCpp
  )} m_${attrData.name};`;
}

function generateCppSetter(attrData) {
  return `
  /**
   * @brief Set the ${attrData.name} of object
   *
   * @param ${attrData.name} The ${attrData.name} of object
   */
  void set${snakeCaseToUpperCamelCase(
    attrData.name
  )}(const ${getCppAttributeType(attrData)} &${attrData.name}) { m_${
    attrData.name
  } = ${attrData.name}; };`;
}

function generateCppGetter(attrData) {
  return `
  /**
   * @brief Get the ${attrData.name} of object
   *
   * @return const ${getCppAttributeType(attrData)}& the ${
    attrData.name
  } of object
   */
  const ${getCppAttributeType(attrData)} &get${snakeCaseToUpperCamelCase(
    attrData.name
  )}() const { return m_${attrData.name}; };`;
}

function generateCppClass(modelClass) {
  const className = modelClass.$.name;
  const extendClass = modelClass.$.extend;
  const filename = `${className}.hxx`;
  const guard = `__GENERATED_MODEL_OBJECT_${className.toUpperCase()}_HXX__`;

  const assessors = [];

  const includesCpp = new Set(["string"]);
  const includesModelObjectsCpp = new Set();
  const attributes = modelClass.attributes[0].attribute;

  if (className === "root_model_object") {
    includesCpp.add("odb/core.hxx");
  }

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
      return generateCppAttribute(
        attrData,
        includesCpp,
        includesModelObjectsCpp
      );
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
      return generateCppAttribute(
        attrData,
        includesCpp,
        includesModelObjectsCpp
      );
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
      return generateCppAttribute(
        attrData,
        includesCpp,
        includesModelObjectsCpp
      );
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
  .filter((inc) => inc !== className)
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
${privateAttributes.length > 0 ? "private:" : ""}
${privateAttributes}

${protectedAttributes.length > 0 ? "protected:" : ""}
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

#pragma db object(${className})

#pragma db view object(${className})
struct ${className}_stat {
#pragma db column("count(" + ${className}::m_id + ")")
  std::size_t count;
};

#endid // end ${guard}`;

  console.info("generated :", cppClassTemplate);
}
function generateTsClass(modelClass) {
  // TODO add template implementation of typescript classes
}

const xml = readFileSync(
  path.resolve(__dirname, "../../../resources/model.xml")
);
parseString(xml, function (err, result) {
  console.info("xml parsing result", JSON.stringify(result, null, 1));
  for (const modelClass of result.model.classes[0].class) {
    allClassNames.add(modelClass.$.name);
  }
  generateClasses(result.model.classes[0].class);
  console.info("generated :", generateCRUDOrderDispatcher(allClassNames));
});
