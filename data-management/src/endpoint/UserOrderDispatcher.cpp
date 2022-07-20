#include "./UserOrderDispatcher.hpp"
#include "./UserDBEndpoint.hpp"
#include <iostream>

/**
 * @brief Route to dispatch user order by object type
 *
 * @param orderType The order type requested
 * @param properties The properties part of request
 * @param data The data part of request
 * @return The result of request process
 */
boost::property_tree::ptree
UserOrderDispatcher::route(const std::string &orderType,
                           const boost::property_tree::ptree &properties,
                           const boost::property_tree::ptree &data) {
  // response data property tree
  boost::property_tree::ptree response;

  std::cout << "ORDER TYPE : " << properties.get<std::string>("order-type")
            << std::endl;
  if ("create" == orderType) {
    // create users from data of property tree
    response = UserDBEndpoint::getInstance()->createUser(data);
  } else if ("read" == orderType) {
    // read users from database, by key of property tree
  } else if ("update" == orderType) {
    // full update of users
  } else if ("delete" == orderType) {
    // delete users
    UserDBEndpoint::getInstance()->deleteUser(data);
  } else if ("patch" == orderType) {
    // partial update of users
  }
  return response;
}
