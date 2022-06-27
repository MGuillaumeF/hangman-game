#ifndef __CRUD_ORDER_DISPATCHER_HPP__
#define __CRUD_ORDER_DISPATCHER_HPP__

#include <boost/property_tree/ptree.hpp>
#include <string>

/**
 * @brief This class goal it's dispatch al order received by order object type
 *
 */
class CRUDOrderDispatcher {
  /**
   * @brief Construct a new CRUD Order Dispatcher object
   * @note deleted
   */
  CRUDOrderDispatcher() = delete;

  /**
   * @brief Destroy the CRUD Order Dispatcher object
   * @note deleted
   */
  ~CRUDOrderDispatcher() = delete;

public:
  /**
   * @brief Route to dispatch crud order by object type
   *
   * @param objectType The object requested
   * @param properties The properties part of request
   * @param data The data part of request
   */
  static void route(const std::string &objectType,
                    const boost::property_tree::ptree &properties,
                    const boost::property_tree::ptree &data);
};

#endif
