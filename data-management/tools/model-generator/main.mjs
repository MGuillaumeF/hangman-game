
import { parseString } from "xml2js";
import { readFileSync } from "fs";
import path from "path";
import {fileURLToPath} from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

function generateClasses(modelClasses) {
  for (const modelClass of modelClasses) {
    generateCppClass(modelClass);
    generateTsClass(modelClass);
  }
}
function generateCppClass(modelClass) {
  const className = modelClass.class.$.name
  const extendClass = modelClass.class.$.extend
  const filename = `${className}.hxx`;
  const guard = `__${className.toUpperCase()}_HXX__`;

  const privateAttributes = modelClass.class.attributes.filter(attributeObject => {
    return attributeObject.attribute.$.visibility == "private";
  });
  const protectedAttributes = modelClass.class.attributes.filter(attributeObject => {
    return attributeObject.attribute.$.visibility == "protected";
  });
  const publicAttributes = modelClass.class.attributes.filter(attributeObject => {
    return attributeObject.attribute.$.visibility == "public";
  });

  const cppClassTemplate = `#ifndef ${guard}
#define ${guard} 

#include <...>

/**
 * @brief class of ${className} in model
 *
 */
#pragma db object
class ${className} ${extendClass ? `final : public ${extendClass}` : ''} {
private:

${privateAttributes.map(attributeObject => {
  const attrData = attributeObject.attribute.$;
  return `${attrData.type} ${attrData.name}`
}).join("\n\n")}

public:
  /**
   * @brief Construct a new ${className} object
   *
   */
  ${className}() = default;



};
#endid // end ${guard}`;

  console.info("generated :", cppClassTemplate);

}
function generateTsClass(modelClass) {

}



const xml = readFileSync(path.resolve(__dirname, "../../resources/model.xml"));
parseString(xml, function (err, result) {
    console.info("xml parsing result", JSON.stringify(result, null, 1));
    // generateClasses(result.model.classes);
});

