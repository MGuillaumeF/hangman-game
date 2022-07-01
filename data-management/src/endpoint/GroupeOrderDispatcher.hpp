#ifndef __GROUPE_ORDER_DISPATCHER_HPP__
#define __GROUPE_ORDER_DISPATCHER_HPP__

#include <boost/property_tree/ptree.hpp>
#include <string>

/**
 * @brief This class goal it's dispatch al order received by order groupe id
 *
 */
class GroupeOrderDispatcher {
  /**
   * @brief Construct a new Groupe Order Dispatcher object
   * @note deleted
   */
  GroupeOrderDispatcher() = delete;
  /**
   * @brief Destroy the Groupe Order Dispatcher object
   * @note deleted
   */
  ~GroupeOrderDispatcher() = delete;

public:
  /**
   * @brief Route to dispatch order by groupe id
   *
   * @param groupeId The groupe id to dispatch requested
   * @param properties The properties part of request
   * @param data The data part of request
   * @return The result of request process
   */
  static boost::property_tree::ptree
  route(const std::string &groupeId,
        const boost::property_tree::ptree &properties,
        const boost::property_tree::ptree &data);
};

#endif
