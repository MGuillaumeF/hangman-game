import { load } from "./loader";
import { ModelAttributesProperties, ModelClassDefinition } from "./modelTypes";
import { snakeCaseToCamelCase, snakeCaseToUpperCamelCase } from "./utils";

/**
 * @class
 * @brief the TypeScript class model generator
 */
export class TypeScriptClassGenerator {
  private static _classNames: Set<string> = new Set<string>();
  private _currentName: string;
  private _motherClass: string | undefined;
  private _dependencies: Set<string> = new Set<string>();

  /**
   * @constructor
   * @brief the constructor of generator of TypeScript model class
   * @param modelClass model class properties
   */
  constructor(modelClass: ModelClassDefinition) {
    this._currentName = modelClass.name;
    if (modelClass.extend) {
      this._motherClass = modelClass.extend;
    }
  }

  /**
   * @brief setter of classNames of TypeScript generator
   * @param the Set of classNames of model
   */
  public static set classNames(value: Set<string>) {
    TypeScriptClassGenerator._classNames = value;
  }

  /**
   * @brief method to generate dependencies import block
   * @returns The string to list dependencies
   */
  public generateDependencies(): string {
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
   * @brief method to generate constraint of string data
   * @param attibuteProperties Properties of string attribute
   * @returns The constraint object of attribute
   */
  private static generateStringConstraint(
    attibuteProperties: ModelAttributesProperties
  ): {
    mandatory?: boolean;
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

  /**
   * @brief method to generate constraint of number data
   * @param attibuteProperties Properties of number attribute
   * @returns The constraint object of attribute
   */
  private static generateNumberConstraint(
    attibuteProperties: ModelAttributesProperties
  ): {
    mandatory?: boolean;
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

  /**
   * @brief method to generate constraint of date data
   * @param attibuteProperties Properties of date attribute
   * @returns The constraint object of attribute
   */
  private static generateDateConstraint(
    attibuteProperties: ModelAttributesProperties
  ): {
    mandatory?: boolean;
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
        const isArrayType = /^.+\[\]$/.test(typeObjectName);

        typeObjectName = isArrayType
          ? typeObjectName.slice(0, -2)
          : typeObjectName;

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
        const isArrayType = /^.+\[\]$/.test(attibuteProperties.type);
        let typeObjectName = isArrayType
          ? attibuteProperties.type.slice(0, -2)
          : attibuteProperties.type;

        if (typeList.includes(typeObjectName)) {
          typeObjectName = String(tsMapTypes[typeObjectName]);

          switch (typeObjectName) {
            case "string":
              if (isArrayType) {
                checks.push(
                  `if (this.${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )} !== undefined) {
               errors.push(...([] as ModelError[]).concat(...this.${snakeCaseToCamelCase(
                 attibuteProperties.name
               )}.map(item => Validator.checkStringProperty(${snakeCaseToUpperCamelCase(
                    this._currentName
                  )}.getConstraintes().${
                    attibuteProperties.name
                  }, "${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )}", item))));
              }`
                );
              } else {
                checks.push(
                  `errors.push(...Validator.checkStringProperty(${snakeCaseToUpperCamelCase(
                    this._currentName
                  )}.getConstraintes().${
                    attibuteProperties.name
                  }, "${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )}", this.${snakeCaseToCamelCase(attibuteProperties.name)}));`
                );
              }
              break;
            case "number":
              if (isArrayType) {
                checks.push(
                  `if (this.${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )} !== undefined) {
               errors.push(...([] as ModelError[]).concat(...this.${snakeCaseToCamelCase(
                 attibuteProperties.name
               )}.map(item => Validator.checkNumberProperty(${snakeCaseToUpperCamelCase(
                    this._currentName
                  )}.getConstraintes().${
                    attibuteProperties.name
                  }, "${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )}", item))));
              }`
                );
              } else {
                checks.push(
                  `errors.push(...Validator.checkNumberProperty(${snakeCaseToUpperCamelCase(
                    this._currentName
                  )}.getConstraintes().${
                    attibuteProperties.name
                  }, "${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )}", this.${snakeCaseToCamelCase(attibuteProperties.name)}));`
                );
              }
              break;
            default:
              console.info("The type haven't constrainte");
          }
        } else if (TypeScriptClassGenerator._classNames.has(typeObjectName)) {
          if (isArrayType) {
            checks.push(
              `if (this.${snakeCaseToCamelCase(
                attibuteProperties.name
              )} !== undefined) {
               errors.push(...([] as ModelError[]).concat(...this.${snakeCaseToCamelCase(
                 attibuteProperties.name
               )}.map(item => item.getErrors())));
              }`
            );
          } else {
            checks.push(
              `if (this.${snakeCaseToCamelCase(
                attibuteProperties.name
              )} !== undefined) {
               errors.push(...this.${snakeCaseToCamelCase(
                 attibuteProperties.name
               )}.getErrors());
              }`
            );
          }
        }
      }
    );
    return `
    /**
     * @brief method to get errors of objects
     * @returns the list of model constraint error
     */
    public getErrors() : ModelError[] {
      const errors : ModelError[] = []
      ${checks.join("\n")}
      return errors;
    }`;
  }

  /**
   * @brief method to generate serializer function
   * @param attibutePropertiesList the list of attributes properties
   * @returns The string of generate serializer function
   */
  public generateSerializer(
    attibutePropertiesList: ModelAttributesProperties[]
  ): string {
    // get all attributes names of object to serializer its
    const attrNamesList = attibutePropertiesList
      .map((attibuteProperties) =>
        snakeCaseToCamelCase(attibuteProperties.name)
      )
      .join(", ");

    // get all attributes with type management to have good convertion
    const attrSerializeList = attibutePropertiesList
      .map((attibuteProperties) => {
        // detect from type if is array type
        const isArrayType = /^.+\[\]$/.test(attibuteProperties.type);
        // get type of data (without array notation if needed
        const rawType = isArrayType
          ? attibuteProperties.type.slice(0, -2)
          : attibuteProperties.type;

        let attributeSerialized: undefined | string;

        if (TypeScriptClassGenerator._classNames.has(rawType)) {
          attributeSerialized = `${attibuteProperties.name} : ${
            isArrayType
              ? `${snakeCaseToCamelCase(
                  attibuteProperties.name
                )} !== undefined ?
                  ${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )}.map(item => item.toJSON()) : undefined`
              : `${snakeCaseToCamelCase(
                  attibuteProperties.name
                )} !== undefined ? ${snakeCaseToCamelCase(
                  attibuteProperties.name
                )}.toJSON() : undefined`
          }`;
        } else {
          attributeSerialized =
            attibuteProperties.name ===
            snakeCaseToCamelCase(attibuteProperties.name)
              ? attibuteProperties.name
              : `${attibuteProperties.name} : ${snakeCaseToCamelCase(
                  attibuteProperties.name
                )}`;
        }
        return attributeSerialized;
      })
      .join(", ");

    return `
    /**
     * @brief method to convert object to JSON object
     * @returns the object json representation of object instance
     */
    public toJSON() : any {
        const {${attrNamesList}} = this;
        return { ${
          this._motherClass !== undefined ? "...super.toJSON()," : ""
        } ${attrSerializeList}}
    }
    `;
  }

  /**
   * @brief method to generate convertor of any object to instance of class
   * @param attibutePropertiesList the list of attributes properties
   * @returns The string of generated convertor method
   */
  public generateParser(
    className: string,
    attibutePropertiesList: ModelAttributesProperties[]
  ): string {
    const buildForObjectArray = (
      attibuteProperties: ModelAttributesProperties,
      rawType: string
    ) => `if (data['${attibuteProperties.name}'] !== undefined) { 
        if (Array.isArray(data['${attibuteProperties.name}'])) {
          obj.${snakeCaseToCamelCase(attibuteProperties.name)} = data['${
      attibuteProperties.name
    }'].map(item => ${snakeCaseToUpperCamelCase(rawType)}.parse(item));
        } else {
          throw Error("INVALID TYPE")
        }
      }`;

    const buildForObject = (
      attibuteProperties: ModelAttributesProperties,
      rawType: string
    ) => `if (data['${attibuteProperties.name}'] !== undefined) { 
        if (typeof data['${attibuteProperties.name}'] === "object") {
          obj.${snakeCaseToCamelCase(
            attibuteProperties.name
          )} = ${snakeCaseToUpperCamelCase(rawType)}.parse(data['${
      attibuteProperties.name
    }'])
        } else {
          throw Error("INVALID TYPE")
        }
      }`;

    const buildForPrimitiveType = (
      attibuteProperties: ModelAttributesProperties
    ) => {
      const isArrayType = /^.+\[\]$/.test(attibuteProperties.type);
      let foundedType = isArrayType
        ? attibuteProperties.type.slice(0, -2)
        : attibuteProperties.type;
      foundedType = Object.keys(tsMapTypes).includes(foundedType)
        ? String(tsMapTypes[foundedType])
        : foundedType;
      let typeAssert = "";
      if (isArrayType) {
        typeAssert = `Array.isArray(data['${
          attibuteProperties.name
        }']) && data['${attibuteProperties.name}'].every((item : unknown) => ${
          foundedType !== "Date"
            ? `typeof item === "${foundedType}"`
            : `item instanceof ${foundedType}`
        })`;
      } else {
        typeAssert =
          foundedType !== "Date"
            ? `typeof data['${attibuteProperties.name}'] === "${foundedType}"`
            : `data['${attibuteProperties.name}'] instanceof ${foundedType}`;
      }

      return `if (data['${attibuteProperties.name}'] !== undefined) { 
                if (${typeAssert}) {
                  obj.${snakeCaseToCamelCase(
                    attibuteProperties.name
                  )} = data['${attibuteProperties.name}'];
                } else {
                  throw Error("INVALID TYPE")
                }
              }`;
    };

    return `
   /**
    * @brief method to generate convertor of any object to instance of class
    * @param data The data to convert to instance
    * @returns The instance of converted object
    */
    public static parse${
      className === "RootModelObject"
        ? `MetaData<T extends ${snakeCaseToUpperCamelCase(
            className
          )}>(obj : T, data : any)`
        : "(data : any)"
    } : ${
      className === "RootModelObject"
        ? "T"
        : snakeCaseToUpperCamelCase(className)
    } {
         ${
           className === "RootModelObject"
             ? ""
             : `const obj = RootModelObject.parseMetaData<${className}>(new ${className}(), data);`
         }
        if (typeof data === "object") {
          ${attibutePropertiesList
            .map((attibuteProperties) => {
              const isArrayType = /^.+\[\]$/.test(attibuteProperties.type);
              const rawType = isArrayType
                ? attibuteProperties.type.slice(0, -2)
                : attibuteProperties.type;

              return TypeScriptClassGenerator._classNames.has(rawType)
                ? isArrayType
                  ? buildForObjectArray(attibuteProperties, rawType)
                  : buildForObject(attibuteProperties, rawType)
                : buildForPrimitiveType(attibuteProperties);
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
  const className = snakeCaseToUpperCamelCase(modelClass.name);
  const generator = new TypeScriptClassGenerator(modelClass);
  const extendClass = modelClass.extend;
  const filename = `${className}.ts`;

  const attributes = modelClass.attributes;
  const publicMethods: string[] = [];
  const privateAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.visibility === "private";
    })
    .map((attributeObject) => {
      publicMethods.push(generator.generateTsSetter(attributeObject));
      publicMethods.push(generator.generateTsGetter(attributeObject));
      return generator.generateTsAttribute(attributeObject);
    })
    .join("\n");

  const protectedAttributes = attributes
    .filter((attributeObject) => {
      return attributeObject.visibility === "protected";
    })
    .map((attributeObject) => {
      publicMethods.push(generator.generateTsSetter(attributeObject));
      publicMethods.push(generator.generateTsGetter(attributeObject));
      return generator.generateTsAttribute(attributeObject);
    })
    .join("\n");

  publicMethods.push(
    generator.generateGetErrorsMethod(attributes),
    generator.generateSerializer(attributes),
    generator.generateParser(className, attributes)
  );
  const generatedTsTemplate = load("TsClasses", {
    abstract: modelClass.abstract === "true" ? "abstract" : "",
    className,
    constraintes: generator.generateAttributesConstraintes(
      attributes,
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
