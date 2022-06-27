#include "./GroupeOrderDispatcher.hpp"
#include "./CRUDOrderDispatcher.hpp"

/**
 * @brief Route to dispatch order by groupe id
 *
 * @param groupeId The groupe id to dispatch requested
 * @param properties The properties part of request
 * @param data The data part of request
 */
void GroupeOrderDispatcher::route(const std::string &groupeId,
                                  const boost::property_tree::ptree &properties,
                                  const boost::property_tree::ptree &data) {
  if ("CRUD" == groupeId) {
    CRUDOrderDispatcher::route(properties.get<std::string>("object-type"),
                               properties, data);
  } else if ("CUSTOM" == groupeId) {

  } else {
    // unknown order groupe
  }
}