#include "./CRUDOrderDispatcher.hpp"

#if defined(DATABASE_MYSQL)
#include "../model/mysql/user-odb.hxx"
#include "../model/mysql/user.hxx"
#elif defined(DATABASE_SQLITE)
#include "../model/sqlite/user-odb.hxx"
#include "../model/sqlite/user.hxx"
#elif defined(DATABASE_PGSQL)
#include "../model/pgsql/user-odb.hxx"
#include "../model/pgsql/user.hxx"
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
    response = routeObjectType<user>(
        user(), properties.get<std::string>("order-type"), properties, data);
  } else if ("word" == objectType) {
    // dispatch crud word orders
    response = routeObjectType<word>(
        word(), properties.get<std::string>("order-type"), properties, data);
  } else {
    // TODO add exception : unknown object type
  }
  return response;
}
