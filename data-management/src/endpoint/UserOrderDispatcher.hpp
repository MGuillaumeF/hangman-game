#ifndef __USER_ORDER_DISPATCHER_HPP__
#define __USER_ORDER_DISPATCHER_HPP__

#include <boost/property_tree/ptree.hpp>
#include <string>

/**
 * @brief This class goal it's dispatch al order received by order type
 *
 */
class UserOrderDispatcher {
  /**
   * @brief Construct a new User Order Dispatcher object
   * @note deleted
   */
  UserOrderDispatcher() = delete;

  /**
   * @brief Destroy the User Order Dispatcher object
   * @note deleted
   */
  ~UserOrderDispatcher() = delete;

public:
  /**
   * @brief Route to dispatch user order by object type
   *
   * @param orderType The order type requested
   * @param properties The properties part of request
   * @param data The data part of request
   */
  static void route(const std::string &orderType,
                    const boost::property_tree::ptree &properties,
                    const boost::property_tree::ptree &data);
};

#endif
