function generateDatabaseImport(databaseType, classNames) {
  const importObjectList = [];
  for (const className of classNames) {
    importObjectList.push(
      `#include "../model/${databaseType}/${className}-odb.hxx"`, 
      `#include "../model/${databaseType}/${className}.hxx"`
    );
  }
  return importObjectList.join('\n');
}

function generateObjectTypesDispatcher(classNames) {
   const conditionList = Array.from(classNames).filter(className => className !== 'root_model_object').map((className, index) => {
   return `${index===0 ? '' : 'else'} if ("${className}" == objectType) {
    // dispatch crud ${className} orders
    response = routeObjectType<${className}>(properties.get<std::string>("order-type"),
                                     properties, data);
  }`});
  return conditionList.join(' ');
}

export function generateCRUDOrderDispatcher(classNames) {


return `
#include "./CRUDOrderDispatcher.hpp"

#if defined(DATABASE_MYSQL)
${generateDatabaseImport("mysql", classNames)}
#elif defined(DATABASE_SQLITE)
${generateDatabaseImport("sqlite", classNames)}
#elif defined(DATABASE_PGSQL)
${generateDatabaseImport("pgsql", classNames)}
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include <iostream>

/**
 * @brief Route to dispatch crud order by object type
 *
 * @param objectType The object requested
 * @param properties The properties part of request
 * @param data The data part of request
 * @return The result of request process
 */
boost::property_tree::ptree
CRUDOrderDispatcher::route(const std::string &objectType,
                           const boost::property_tree::ptree &properties,
                           const boost::property_tree::ptree &data) {
  boost::property_tree::ptree response;

  std::cout << "OBJECT TYPE : " << properties.get<std::string>("object-type")
            << std::endl;
  ${generateObjectTypesDispatcher(classNames)} else {
    // unknown object type
    response.put("status-code", uint16_t(status_code::NOT_FOUND));
  }
  return response;
}
`;

}
