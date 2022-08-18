import { load } from "./loader.mjs";

function generateDatabaseImport(databaseType, classNames) {
  const importObjectList = [];
  for (const className of classNames) {
    importObjectList.push(
      `#include "../model/${databaseType}/${className}-odb.hxx"`,
      `#include "../model/${databaseType}/${className}.hxx"`
    );
  }
  return importObjectList.join("\n");
}

function generatrDatabaseImportBloc(databaseTypes, classNames) {
  const blocString = databaseTypes.map(
    (databaseType, index) => `
#${index === 0 ? "" : "el"}if defined(DATABASE_${databaseType.toUpperCase()})
${generateDatabaseImport(databaseType, classNames)}
`
  );
  blocString.push(`
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif`);
  return blocString.join("");
}

function generateObjectTypesDispatcher(classNames) {
  const conditionList = Array.from(classNames)
    .filter((className) => className !== "root_model_object")
    .map((className, index) => {
      return `${index === 0 ? "" : "else"} if ("${className}" == objectType) {
    // dispatch crud ${className} orders
    response = routeObjectType<${className}>(properties.get<std::string>("order-type"),
                                     properties, data);
  }`;
    });
  return conditionList.join(" ");
}

export function generateCRUDOrderDispatcher(classNames) {
  return load("CRUDOrderDispatcher", {
    generatedDatabaseImport: generatrDatabaseImportBloc(
      ["mysql", "sqlite", "pgsql", "oracle", "mssql"],
      classNames
    ),
    generatedObjectTypesDispatcher: generateObjectTypesDispatcher(classNames),
  });
}
