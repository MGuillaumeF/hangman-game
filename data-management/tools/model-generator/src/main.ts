import { mkdirSync, readFileSync, writeFileSync } from "fs";
import { resolve } from "path";
import { parseString } from "xml2js";
import { generateCRUDOrderDispatcher } from "./generateCRUDOrderDispatcher";
import { generateTsClass, TypeScriptClassGenerator } from "./generateTsClass";
import { ModelAttributesProperties, ModelClassDefinition } from "./modelTypes";
import { snakeCaseToUpperCamelCase } from "./utils";

const cppMapTypes: { [key: string]: string } = {
  date: "std::time_t",
  string: "std::string",
  int8: "int8_t",
  int16: "int16_t",
  int32: "int32_t",
  int64: "int64_t",
  uint8: "uint8_t",
  uint16: "uint16_t",
  uint32: "uint32_t",
  uint64: "uint64_t"
};

const allClassNames = new Set<string>();

const cppMapIncludes: { [key: string]: string } = {
  date: "ctime",
  string: "string"
};

/**
 *
 * @param attrData
 * @param includesLib
 * @param includesObjects
 * @returns
 */
function getCppAttributeType(
  attrData: ModelAttributesProperties,
  includesLib?: Set<string>,
  includesObjects?: Set<string>
) {
  let attributeType = attrData.type;
  const isArray = /^.*\[\]$/.test(attributeType);
  if (isArray) {
    attributeType = attributeType.slice(0, -2);
    if (includesLib) {
      includesLib.add("vector");
    }
  }

  const attributeTypeIncludeMapped = cppMapIncludes[attributeType];
  if (includesLib && attributeTypeIncludeMapped) {
    includesLib.add(attributeTypeIncludeMapped);
  }

  const attributeTypeMapped = cppMapTypes[attributeType];
  if (attributeTypeMapped) {
    attributeType = attributeTypeMapped;
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

/**
 *
 * @param {*} modelClasses
 */
function generateClasses(modelClasses: ModelClassDefinition[]) {
  for (const modelClass of modelClasses) {
    const modelCppDirPath = resolve("dist", "cpp", "model");
    const modelTsDirPath = resolve("dist", "ts", "model");
    mkdirSync(modelCppDirPath, { recursive: true });
    mkdirSync(modelTsDirPath, { recursive: true });
    writeFileSync(
      resolve("dist", "cpp", "model", `${modelClass.$.name}.hxx`),
      generateCppClass(modelClass)
    );
    writeFileSync(
      resolve(
        modelTsDirPath,
        `${snakeCaseToUpperCamelCase(modelClass.$.name)}.ts`
      ),
      generateTsClass(modelClass)
    );
  }
}

/**
 *
 * @param attrData
 * @returns
 */
function generateCppPragma(attrData: ModelAttributesProperties) {
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

/**
 *
 * @param attrData
 * @param includesCpp
 * @param includesModelObjectsCpp
 * @returns
 */
function generateCppAttribute(
  attrData: ModelAttributesProperties,
  includesCpp?: Set<string>,
  includesModelObjectsCpp?: Set<string>
) {
  return `${generateCppPragma(attrData)}${getCppAttributeType(
    attrData,
    includesCpp,
    includesModelObjectsCpp
  )} m_${attrData.name};`;
}

/**
 *
 * @param {*} attrData
 * @returns
 */
function generateCppSetter(attrData: ModelAttributesProperties) {
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

/**
 *
 * @param attrData
 * @returns
 */
function generateCppGetter(attrData: ModelAttributesProperties) {
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

/**
 *
 * @param modelClass
 */
function generateCppClass(modelClass: ModelClassDefinition) {
  const className = modelClass.$.name;
  const extendClass = modelClass.$.extend;
  const filename = `${className}.hxx`;
  const guard = `__GENERATED_MODEL_OBJECT_${className.toUpperCase()}_HXX__`;

  const assessors: string[] = [];

  const includesCpp = new Set(["string"]);
  const includesModelObjectsCpp = new Set<string>();
  const attributes: {
    $: ModelAttributesProperties;
  }[] = modelClass.attributes[0]?.attribute || [];

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
  return cppClassTemplate;
}

// "../../../resources/model.xml"
const xmlPath = resolve(
  process.argv[2] ? process.argv[2] : "../../../resources/model.xml"
);
console.log("reading file : ", xmlPath);
const xml = readFileSync(xmlPath);

/**
ModelAttributesProperties = {
  cardinality?: UnidirectionalCardinality | BidirectionalCardinality;
  linked_column?: string;
  mandatory: boolean;
  name: string;
  pattern: string;
  type: string;
  visibility: "private" | "protected" | "public";
};
*/

function isModelAttributesProperties(
  data: any
): data is ModelAttributesProperties {
  return (
    typeof data === "object" &&
    data !== null &&
    [data["max"], data["max_length"], data["min"], data["min_length"]].every(
      (dataItem) => {
        let isValid = ["string", "undefined"].includes(typeof dataItem);
        if (typeof dataItem === "string" && Number.isNaN(Number(dataItem))) {
          isValid = false;
        }
        return isValid;
      }
    )
  );
}

function isModelClassDefinition(data: any): data is ModelClassDefinition {
  let result = true;
  if (typeof data === "object" && data !== null) {
    result =
      result &&
      typeof data["$"] === "object" &&
      data["$"] !== null &&
      typeof data["$"]["name"] === "string";

    result =
      result &&
      Array.isArray(data["attributes"]) &&
      data["attributes"].length === 1 &&
      typeof data["attributes"][0] === "object" &&
      data["attributes"][0] !== null &&
      Array.isArray(data["attributes"][0]["attribute"]) &&
      data["attributes"][0]["attribute"]
        .map((attributDefContainer) =>
          typeof attributDefContainer === "object" &&
          attributDefContainer !== null
            ? attributDefContainer["$"]
            : undefined
        )
        .every(isModelAttributesProperties);
  } else {
    result = false;
  }

  return result;
}

function isModelClassDefinitionList(
  datas: any
): datas is ModelClassDefinition[] {
  return Array.isArray(datas) && datas.every(isModelClassDefinition);
}

parseString(xml, function (err, result) {
  console.info("xml parsing result", JSON.stringify(result, null, 1));
  for (const modelClass of result.model.classes[0].class.filter(
    isModelClassDefinitionList
  )) {
    allClassNames.add(modelClass.$.name);
  }
  TypeScriptClassGenerator.classNames = allClassNames;
  generateClasses(result.model.classes[0].class);
  const endpointDirPath = resolve("dist", "cpp", "endpoint");
  const endpointPath = resolve(
    endpointDirPath,
    "generateCRUDOrderDispatcher.cpp"
  );
  mkdirSync(endpointDirPath, { recursive: true });
  const generatedCRUDOrderDispatcher =
    generateCRUDOrderDispatcher(allClassNames);
  writeFileSync(endpointPath, generatedCRUDOrderDispatcher);
  console.info("generated :", generatedCRUDOrderDispatcher);
});
