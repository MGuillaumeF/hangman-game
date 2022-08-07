#include "./GroupeOrderDispatcher.hpp"
#include "./CRUDOrderDispatcher.hpp"

/**
 * @brief Route to dispatch order by groupe id
 *
 * @param groupeId The groupe id to dispatch requested
 * @param properties The properties part of request
 * @param data The data part of request
 * @return The result of request process
 */
boost::property_tree::ptree
GroupeOrderDispatcher::route(const std::string &groupeId,
                             const boost::property_tree::ptree &properties,
                             const boost::property_tree::ptree &data) {
  boost::property_tree::ptree response;
  if ("CRUD" == groupeId) {
    response = CRUDOrderDispatcher::route(
        properties.get<std::string>("object-type"), properties, data);
  } else if ("CUSTOM" == groupeId) {

  } else {
    // unknown order group
    // unknown object type
    response.put("status-code", status_code::NOT_FOUND);
  }
  return response;
}
