import { load } from "./loader";
import { ModelAttributesProperties, ModelClassDefinition } from "./modelTypes";
import { snakeCaseToCamelCase, snakeCaseToUpperCamelCase } from "./utils";

/**
 *
 */
export class TypeScriptClassGenerator {
  private static _classNames: Set<string> = new Set<string>();
  private _currentName: string;
  private _motherClass: string | undefined;
  private _dependencies: Set<string> = new Set<string>();

  /**
   *
   * @param modelClass
   */
  constructor(modelClass: ModelClassDefinition) {
    this._currentName = modelClass.$.name;
    if (modelClass.$.extend) {
      this._motherClass = modelClass.$.extend;
    }
  }

  /**
   *
   */
  public static set classNames(value: Set<string>) {
    TypeScriptClassGenerator._classNames = value;
  }

  /**
   *
   * @returns
   */
  public generateDependencies() {
    const dependencies = Array.from(this._dependencies);
    if (this._motherClass) {
      dependencies.unshift(snakeCaseToUpperCamelCase(this._motherClass));
    }
    return dependencies
      .filter((name) => name !== snakeCaseToUpperCamelCase(this._currentName))
      .map((dependence) => {
        return `import {${dependence}} from "./${dependence}";`;
      })
      .join("\n");
  }

  /**
   *
   * @param attibuteProperties
   * @returns
   */
  private static generateStringConstraint(
    attibuteProperties: ModelAttributesProperties
  ): {
    mandatory: boolean;
    max_length?: number;
    min_length?: number;
    pattern?: string;
    type: "string";
  } {
    return {
      mandatory: attibuteProperties.mandatory,
      max_length: attibuteProperties.max_length,
      min_length: attibuteProperties.min_length,
      pattern: attibuteProperties.pattern,
      type: "string"
    };
  }
  private static generateNumberConstraint(
    attibuteProperties: ModelAttributesProperties
  ): {
    mandatory: boolean;
    max?: number;
    min?: number;
    type: "number";
  } {
    return {
      mandatory: attibuteProperties.mandatory,
      max: attibuteProperties.max,
      min: attibuteProperties.min,
      type: "number"
    };
  }
  private static generateDateConstraint(
    attibuteProperties: ModelAttributesProperties
  ): {
    mandatory: boolean;
    max?: number;
    min?: number;
    type: "Date";
  } {
    return {
      mandatory: attibuteProperties.mandatory,
      max: attibuteProperties.max,
      min: attibuteProperties.min,
      type: "Date"
    };
  }

  public generateAttributesConstraintes(
    attibutePropertiesList: ModelAttributesProperties[],
    extendedClassName?: string
  ): string {
    const constraintes: { [key: string]: any } = {};
    const typeList = Object.keys(tsMapTypes);
    attibutePropertiesList.forEach(
      (attibuteProperties: ModelAttributesProperties) => {
        let typeObjectName = attibuteProperties.type;
        if (typeList.includes(typeObjectName)) {
          typeObjectName = String(tsMapTypes[typeObjectName]);
        }
        switch (typeObjectName) {
          case "string":
            constraintes[attibuteProperties.name] =
              TypeScriptClassGenerator.generateStringConstraint(
                attibuteProperties
              );
            break;
          case "number":
            constraintes[attibuteProperties.name] =
              TypeScriptClassGenerator.generateNumberConstraint(
                attibuteProperties
              );
            break;
          case "Date":
            constraintes[attibuteProperties.name] =
              TypeScriptClassGenerator.generateDateConstraint(
                attibuteProperties
              );
            break;
          default:
            console.info("The type haven't constrainte");
        }
      }
    );
    return `protected static getConstraintes () {
      return ${
        extendedClassName
          ? `{...${snakeCaseToUpperCamelCase(
              extendedClassName
            )}.getConstraintes(), ...${JSON.stringify(constraintes, null, 4)}}`
          : JSON.stringify(constraintes, null, 4)
      };
    } `;
  }

  /**
   *
   * @param attibutePropertiesList
   * @returns
   */
  public generateGetErrorsMethod(
    attibutePropertiesList: ModelAttributesProperties[]
  ) {
    const checks: string[] = [];
    const typeList = Object.keys(tsMapTypes);
    this._dependencies.add("Validator");
    this._dependencies.add("ModelError");
    attibutePropertiesList.forEach(
      (attibuteProperties: ModelAttributesProperties) => {
        let typeObjectName = attibuteProperties.type;
        if (typeList.includes(typeObjectName)) {
          typeObjectName = String(tsMapTypes[typeObjectName]);
        }
        switch (typeObjectName) {
          case "string":
            checks.push(
              `errors.push(...Validator.checkStringProperty(${snakeCaseToUpperCamelCase(
                this._currentName
              )}.getConstraintes().${
                attibuteProperties.name
              }, this.${snakeCaseToCamelCase(attibuteProperties.name)}));`
            );
            break;
          case "number":
            this._dependencies.add("Validator");
            checks.push(
              `errors.push(...Validator.checkNumberProperty(${snakeCaseToUpperCamelCase(
                this._currentName
              )}.getConstraintes().${
                attibuteProperties.name
              }, this.${snakeCaseToCamelCase(attibuteProperties.name)}));`
            );
            break;
          default:
            console.info("The type haven't constrainte");
        }
      }
    );
    return `
    public getErrors() : ModelError[] {
      const errors : ModelError[] = []
      ${checks.join("\n")}
      return errors;
    }`;
  }

  /**
   *
   * @param attibutePropertiesList
   * @returns
   */
  public generateSerializer(
    attibutePropertiesList: ModelAttributesProperties[]
  ): string {
    const attrNamesList = attibutePropertiesList
      .map((attibuteProperties) =>
        snakeCaseToCamelCase(attibuteProperties.name)      )
      .join(", ");
    const attrSerializeList = attibutePropertiesList
      .map((attibuteProperties) => {
          const isArrayType = /^.+\[\]$/.test(attibuteProperties.type);
          const rawType = isArrayType ? attibuteProperties.type.slice(0,-2) : attibuteProperties.type
          
          return TypeScriptClassGenerator._classNames.has(rawType) ? `${snakeCaseToCamelCase(attibuteProperties.name)} : this.${snakeCaseToCamelCase(attibuteProperties.name)}.toJSON()` : snakeCaseToCamelCase(attibuteProperties.name)
        }
      )
      .join(", ");

    return `
    public toJSON() {
        const {${attrNamesList}} = this;
        return `{${attrSerializeList}}`
    }
    `;
  }

  /**
   *
   * @param attibutePropertiesList
   * @returns
   */
  public generateParser(
    className: string,
    attibutePropertiesList: ModelAttributesProperties[]
  ): string {
    return `
    public static parse(data : any) : ${snakeCaseToUpperCamelCase(className)} {
        const obj = new ${snakeCaseToUpperCamelCase(className)}();
        if (typeof data === "object") {
          ${attibutePropertiesList
            .map((attibuteProperties) => {
              return `if (data['${attibuteProperties.name}'] !== undefined) {
              if (typeof data['${attibuteProperties.name}'] === "${
                attibuteProperties.type
              }") {
                obj.${snakeCaseToCamelCase(attibuteProperties.name)} = data['${
                attibuteProperties.name
              }'];
              } else {
                throw Error("INVALID TYPE")
              }
            }`;
            })
            .join("\n")}
        }
        return obj
    }
    `;
  }

  /**
   *
   * @param attibuteProperties
   * @returns
   */
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
    return isArray
      ? `${typeObjectName}[] | undefined`
      : `${typeObjectName} | undefined`;
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
    )}) { this._${snakeCaseToCamelCase(attrData.name)} = value; }`;
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
    )} { return this._${snakeCaseToCamelCase(attrData.name)}; }`;
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
  bool: "boolean",
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
  const generator = new TypeScriptClassGenerator(modelClass);
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

  publicMethods.push(
    generator.generateGetErrorsMethod(attributes.map((attr) => attr.$)),
    generator.generateSerializer(attributes.map((attr) => attr.$)),
    generator.generateParser(
      className,
      attributes.map((attr) => attr.$)
    )
  );
  const generatedTsTemplate = load("TsClasses", {
    className,
    constraintes: generator.generateAttributesConstraintes(
      attributes.map((attr) => attr.$),
      extendClass
    ),
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
