import { mkdirSync, readFileSync, writeFileSync } from "fs";
import { resolve } from "path";
import { parseString } from "xml2js";
import { generateCRUDOrderDispatcher } from "./generateCRUDOrderDispatcher";
import { TypeScriptClassGenerator, generateTsClass } from "./generateTsClass";
import {
  ModelAttributesProperties,
  ModelClassDefinition,
  XMLModelAttributesProperties,
  XMLModelClassDefinition
} from "./modelTypes";
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
  includesObjects?: Set<string>,
  declareModelObjectsCpp?: Set<string>
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
    if (declareModelObjectsCpp && attrData.linked_column) {
      declareModelObjectsCpp.add(attributeType);
    } else {
      includesObjects.add(attributeType);
    }
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
    mkdirSync(modelCppDirPath, { recursive: true });
    writeFileSync(
      resolve("dist", "cpp", "model", `${modelClass.name}.hxx`),
      generateCppClass(modelClass)
    );
    if (!process.argv.includes("--cpp-only")) {
      const modelTsDirPath = resolve("dist", "ts", "model");
      mkdirSync(modelTsDirPath, { recursive: true });
      writeFileSync(
        resolve(
          modelTsDirPath,
          `${snakeCaseToUpperCamelCase(modelClass.name)}.ts`
        ),
        generateTsClass(modelClass)
      );
    }
  }
}

/**
 *
 * @param attrData
 * @returns
 */
function generateCppPragma(attrData: ModelAttributesProperties) {
  const pragmas = [];
  if (attrData.object_id && attrData.macro === "increment-auto") {
    pragmas.push("id", "auto");
  }
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
  includesModelObjectsCpp?: Set<string>,
  declareModelObjectsCpp?: Set<string>
) {
  let defaultValue =
    attrData.default_value !== undefined ? attrData.default_value : "";
  if (defaultValue !== "") {
    if (attrData.type === "string") {
      defaultValue = `"${defaultValue}"`;
    }
    if (attrData.type === "date" && defaultValue === "current") {
      defaultValue = `std::time(nullptr)`;
    }
    defaultValue = `= ${defaultValue}`;
  }
  return `${generateCppPragma(attrData)}${getCppAttributeType(
    attrData,
    includesCpp,
    includesModelObjectsCpp,
    declareModelObjectsCpp
  )} m_${attrData.name}${defaultValue};`;
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

function generateParser(
  modelClassName: string,
  attrData: ModelAttributesProperties[]
) {
  const objParsing = attrData.map((attribute) => {
    const isArray = /^.*\[\]$/.test(attribute.type);
    if (isArray) {
      return "";
    } else if (allClassNames.has(attribute.type)) {
      return "";
      return `const boost::optional<const boost::property_tree::ptree&> ${
        attribute.name
      }_obj =
          property_tree.get_child_optional("${attribute.name}");
      if (${attribute.name}_obj) {
        parsedObject->set${snakeCaseToUpperCamelCase(attribute.name)}(${
          attribute.type
        }::parse(*${attribute.name}_obj));
      }`;
    } else {
      const attributeTypeMapped = cppMapTypes[attribute.type]
        ? cppMapTypes[attribute.type]
        : attribute.type;

      return `const boost::optional<${attributeTypeMapped}> ${attribute.name} =
          property_tree.get_optional<${attributeTypeMapped}>("${
            attribute.name
          }");
      if (${attribute.name}) {
        parsedObject->set${snakeCaseToUpperCamelCase(attribute.name)}(*${
          attribute.name
        });
      }`;
    }
  });

  return `/**
   * @brief method to extract object from property tree
   *
   * @return The ${modelClassName} found
   */
  static std::unique_ptr<${modelClassName}> parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<${modelClassName}> parsedObject = root_model_object::parse<${modelClassName}>(property_tree);
    ${objParsing.join("\n")}
    return parsedObject;
  }`;
}

function generateCppGetErrors(
  modelClassName: string,
  attrData: ModelAttributesProperties[]
) {
  return `
   /**
   * @brief method to check if all fields of object are valid
   *
   * @return the error vector of validation
   */
  std::vector<model_error> getErrors() const {
    std::vector<model_error> errors;
    // TODO add implementation
    return errors;
  }`;
}

/**
 *
 * @param modelClass
 */
function generateCppClass(modelClass: ModelClassDefinition) {
  const className = modelClass.name;
  const extendClass = modelClass.extend;
  const filename = `${className}.hxx`;
  const guard = `__GENERATED_MODEL_OBJECT_${className.toUpperCase()}_HXX__`;

  const assessors: string[] = [];

  const includesCpp = new Set(["string"]);
  const includesModelObjectsCpp = new Set<string>();
  const declareModelObjectsCpp = new Set<string>();
  const { attributes } = modelClass;

  if (className === "root_model_object") {
    includesCpp.add("odb/core.hxx");
  }

  if (extendClass) {
    includesModelObjectsCpp.add(extendClass);
  }

  const privateAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.visibility == "private";
    })
    .map((attributeObject) => {
      assessors.push(generateCppSetter(attributeObject));
      assessors.push(generateCppGetter(attributeObject));
      return generateCppAttribute(
        attributeObject,
        includesCpp,
        includesModelObjectsCpp,
        declareModelObjectsCpp
      );
    })
    .join("\n");
  const protectedAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.visibility == "protected";
    })
    .map((attributeObject) => {
      assessors.push(generateCppSetter(attributeObject));
      assessors.push(generateCppGetter(attributeObject));
      return generateCppAttribute(
        attributeObject,
        includesCpp,
        includesModelObjectsCpp,
        declareModelObjectsCpp
      );
    })
    .join("\n");
  const publicAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.visibility == "public";
    })
    .map((attributeObject) => {
      assessors.push(generateCppSetter(attributeObject));
      assessors.push(generateCppGetter(attributeObject));
      return generateCppAttribute(
        attributeObject,
        includesCpp,
        includesModelObjectsCpp,
        declareModelObjectsCpp
      );
    })
    .join("\n");

  const cppClassTemplate = `
/**
 * @brief ${filename} DO NOT MODIFY THIS FILE, this file is a generated model class
 */

#ifndef ${guard}
#define ${guard} 

${Array.from(includesModelObjectsCpp)
  .filter((inc) => inc !== className)
  .map((inc) => `#include "./${inc}.hxx"`)
  .join("\n")}

  ${
    className === "root_model_object"
      ? '#include "./model_error.hpp"\n#include <boost/property_tree/ptree.hpp>\n#include <list>\n#include <vector>'
      : ""
  }
${Array.from(includesCpp)
  .map((inc) => `#include <${inc}>`)
  .join("\n")}

${Array.from(
  new Set<string>([
    ...Array.from(declareModelObjectsCpp),
    ...Array.from(includesModelObjectsCpp)
  ])
)
  .filter((inc) => inc !== className && inc !== "root_model_object")
  .map((inc) => `class ${inc};`)
  .join("\n")}

/**
 * @brief class of ${className} object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class ${className} ${extendClass ? `final : public ${extendClass}` : ""} {
private:
friend class odb::access;
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

  ${
    className === "root_model_object"
      ? `
  /**
   * @brief method to convert object to property tree
   *
   * @return The object on property tree format
   */
  boost::property_tree::ptree toPtree();

  /**
   * @brief method to extract object from property tree
   *
   * @return The object found
   */
  template <typename T, typename std::enable_if_t<std::is_base_of<
                            root_model_object, T>::value> * = nullptr>
  static std::unique_ptr<T> parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<T> childObject = std::make_unique<T>();
    const boost::optional<uint32_t> id =
        property_tree.get_optional<uint32_t>("id");
    if (id) {
      childObject->setId(*id);
    }
    const boost::optional<uint32_t> version =
        property_tree.get_optional<uint32_t>("version");
    if (version) {
      childObject->setVersion(*version);
    }
    const boost::optional<std::string> created_by =
        property_tree.get_optional<std::string>("created_by");
    if (created_by) {
      childObject->setCreatedBy(*created_by);
    }
    const boost::optional<std::string> updated_by =
        property_tree.get_optional<std::string>("updated_by");
    if (updated_by) {
      childObject->setUpdatedBy(*updated_by);
    }
    return childObject;
  };

  /**
   * @brief method of pre create in database
   *
   * @param author The author of new object
   */
  void preCreate(const std::string &author) {
    setVersion(1);
    setCreatedBy(author);
    setCreatedAt(std::time(nullptr));
    setUpdatedBy(author);
    setUpdatedAt(std::time(nullptr));
  }

  /**
   * @brief method of pre update in database
   *
   * @param author The author of update of object
   */
  void preUpdate(const std::string &author) {
    setVersion(getVersion() + 1);
    setUpdatedBy(author);
    setUpdatedAt(std::time(nullptr));
  }
  `
      : generateParser(className, attributes)
  }

  ${generateCppGetErrors(className, attributes)}

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

#endif // end ${guard}

#ifdef ODB_COMPILER
${Array.from(declareModelObjectsCpp)
  .map((inc) => `#include "./${inc}.hxx"`)
  .join("\n")}
#endif

`;

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

function isXMLModelAttributesProperties(
  data: any
): data is XMLModelAttributesProperties {
  return (
    typeof data === "object" &&
    data !== null &&
    ["max", "max_length", "min", "min_length"].every((key) => {
      let isValid = ["string", "undefined"].includes(typeof data[key]);
      if (typeof data[key] === "string" && Number.isNaN(Number(data[key]))) {
        isValid = false;
      }
      console.log("key", key, isValid);
      return isValid;
    })
  );
}

function isXMLModelClassDefinition(data: any): data is XMLModelClassDefinition {
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
        .every(isXMLModelAttributesProperties);
  } else {
    result = false;
  }

  return result;
}

function isXMLModelClassDefinitionList(
  datas: any
): datas is XMLModelClassDefinition[] {
  return Array.isArray(datas) && datas.every(isXMLModelClassDefinition);
}

function xmlToInternalModelClassDefinitionList(
  datas: XMLModelClassDefinition[]
): ModelClassDefinition[] {
  const result: ModelClassDefinition[] = [];
  datas.forEach((data) => {
    if (
      Array.isArray(data?.attributes) &&
      data.attributes[0] &&
      Array.isArray(data.attributes[0].attribute)
    ) {
      const modelAttributesList: ModelAttributesProperties[] =
        data.attributes[0].attribute
          .map((attr) => attr.$)
          .map((attr): ModelAttributesProperties => {
            return {
              ...attr,
              mandatory: attr.mandatory === "true",
              max: attr.max !== undefined ? Number(attr.max) : undefined,
              max_length:
                attr.max_length !== undefined
                  ? Number(attr.max_length)
                  : undefined,
              min: attr.min !== undefined ? Number(attr.min) : undefined,
              min_length:
                attr.min_length !== undefined
                  ? Number(attr.min_length)
                  : undefined,
              object_id: attr.object_id === "true"
            };
          });
      result.push({ ...data.$, attributes: modelAttributesList });
    }
  });

  return result;
}

parseString(xml, function (err, result) {
  console.info("xml parsing result", JSON.stringify(result, null, 1));
  if (
    Array.isArray(result?.model?.classes) &&
    result.model.classes[0] &&
    isXMLModelClassDefinitionList(result.model.classes[0]?.class)
  ) {
    const modelClasses = xmlToInternalModelClassDefinitionList(
      result?.model?.classes[0]?.class
    );

    for (const modelClass of modelClasses) {
      allClassNames.add(modelClass.name);
    }
    TypeScriptClassGenerator.classNames = allClassNames;
    generateClasses(modelClasses);
    const endpointDirPath = resolve("dist", "cpp", "endpoint");
    const endpointPath = resolve(endpointDirPath, "CRUDOrderDispatcher.cpp");
    mkdirSync(endpointDirPath, { recursive: true });
    const generatedCRUDOrderDispatcher =
      generateCRUDOrderDispatcher(allClassNames);
    writeFileSync(endpointPath, generatedCRUDOrderDispatcher);
    console.info("generated :", generatedCRUDOrderDispatcher);
  } else {
    console.log("ee2");
  }
});
