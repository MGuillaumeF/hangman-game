#include "./UserOrderDispatcher.hpp"
#include "./UserDBEndpoint.hpp"
#include <iostream>

/**
 * @brief Route to dispatch user order by object type
 *
 * @param orderType The order type requested
 * @param properties The properties part of request
 * @param data The data part of request
 */
void UserOrderDispatcher::route(const std::string &orderType,
                                const boost::property_tree::ptree &properties,
                                const boost::property_tree::ptree &data) {

  std::cout << "ORDER TYPE : " << properties.get<std::string>("order-type")
            << std::endl;
  if ("create" == orderType) {
    UserDBEndpoint::getInstance()->createUser(data.get_child("user"));
  } else if ("read" == orderType) {

  } else if ("update" == orderType) {

  } else if ("delete" == orderType) {

  } else if ("patch" == orderType) {
  }
}