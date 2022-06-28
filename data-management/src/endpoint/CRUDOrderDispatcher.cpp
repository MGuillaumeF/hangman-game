#include "./CRUDOrderDispatcher.hpp"
#include "./UserOrderDispatcher.hpp"

#include <iostream>

/**
 * @brief Route to dispatch crud order by object type
 *
 * @param objectType The object requested
 * @param properties The properties part of request
 * @param data The data part of request
 */
void CRUDOrderDispatcher::route(const std::string &objectType,
                                const boost::property_tree::ptree &properties,
                                const boost::property_tree::ptree &data) {

  std::cout << "OBJECT TYPE : " << properties.get<std::string>("object-type")
            << std::endl;
  if ("user" == objectType) {
    UserOrderDispatcher::route(properties.get<std::string>("order-type"),
                               properties, data);
  } else if ("word" == objectType) {
    // TODO add WordOrderDispatcher
  } else {
    // TODO add exception : unknown object type
  }
}