#include "./CRUDOrderDispatcher.hpp"

#if defined(DATABASE_MYSQL)
#include "../model/mysql/dictionary-odb.hxx"
#include "../model/mysql/dictionary.hxx"
#include "../model/mysql/group-odb.hxx"
#include "../model/mysql/group.hxx"
#include "../model/mysql/user-odb.hxx"
#include "../model/mysql/user.hxx"
#include "../model/mysql/word-odb.hxx"
#include "../model/mysql/word.hxx"
#elif defined(DATABASE_SQLITE)
#include "../model/sqlite/dictionary-odb.hxx"
#include "../model/sqlite/dictionary.hxx"
#include "../model/sqlite/group-odb.hxx"
#include "../model/sqlite/group.hxx"
#include "../model/sqlite/user-odb.hxx"
#include "../model/sqlite/user.hxx"
#include "../model/sqlite/word-odb.hxx"
#include "../model/sqlite/word.hxx"
#elif defined(DATABASE_PGSQL)
#include "../model/pgsql/dictionary-odb.hxx"
#include "../model/pgsql/dictionary.hxx"
#include "../model/pgsql/group-odb.hxx"
#include "../model/pgsql/group.hxx"
#include "../model/pgsql/user-odb.hxx"
#include "../model/pgsql/user.hxx"
#include "../model/pgsql/word-odb.hxx"
#include "../model/pgsql/word.hxx"
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
  if ("user" == objectType) {
    // dispatch crud user orders
    response = routeObjectType<user>(properties.get<std::string>("order-type"),
                                     properties, data);
  } else if ("word" == objectType) {
    // dispatch crud word orders
    response = routeObjectType<word>(properties.get<std::string>("order-type"),
                                     properties, data);
  } else if ("dictionary" == objectType) {
    // dispatch crud dictionary orders
    response = routeObjectType<dictionary>(
        properties.get<std::string>("order-type"), properties, data);
  } else if ("group" == objectType) {
    // dispatch crud group orders
    response = routeObjectType<group>(properties.get<std::string>("order-type"),
                                      properties, data);
  } else {
    // unknown object type
    response.put("status-code", uint16_t(status_code::NOT_FOUND));
  }
  return response;
}
