#ifndef __CRUD_ORDER_DISPATCHER_HPP__
#define __CRUD_ORDER_DISPATCHER_HPP__

#include "../model/root_model_object.hpp"
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
   * @return The result of request process
   */
  static boost::property_tree::ptree
  route(const std::string &objectType,
        const boost::property_tree::ptree &properties,
        const boost::property_tree::ptree &data);

  /**
   * @brief route to dispatch order by object type
   *
   * @param orderType The order type requested
   * @param properties The properties part of request
   * @param data The data part of request
   * @return The result of request process
   */
  template <typename T, typename std::enable_if<std::is_base_of<
                            root_model_object, T>::value>::type * = nullptr>
  static boost::property_tree::ptree
  routeObjectType(const std::string &orderType,
                  const boost::property_tree::ptree &properties,
                  const boost::property_tree::ptree &data) {
    // response data property tree
    boost::property_tree::ptree response;

    std::cout << "ORDER TYPE : " << properties.get<std::string>("order-type")
              << std::endl;
    if ("create" == orderType) {
      // create object from data of property tree
      response = createObject<T>(properties, data);
    } else if ("read" == orderType) {
      // read from database, by key of property tree
      response = readObject<T>(properties, data);
    } else if ("update" == orderType) {
      // full update with full replace
      response = updateObject<T>(properties, data);
    } else if ("delete" == orderType) {
      // delete object
      response = deleteObject<T>(properties, data);
    } else if ("patch" == orderType) {
      // partial update of users
      response = updateObject<T>(properties, data, false);
    }
    return response;
  }

  /**
   * @brief order to create object
   *
   * @param properties The properties part of request
   * @param data The data part of request
   * @return The result of request process
   */
  template <typename T, typename std::enable_if<std::is_base_of<
                            root_model_object, T>::value>::type * = nullptr>
  static boost::property_tree::ptree
  createObject(const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;

    return response;
  }

  /**
   * @brief order to update object
   *
   * @param properties The properties part of request
   * @param data The data part of request
   * @param full The update is a full replace if true
   * @return The result of request process
   */
  template <typename T, typename std::enable_if<std::is_base_of<
                            root_model_object, T>::value>::type * = nullptr>
  static boost::property_tree::ptree
  updateObject(const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data, bool full = true) {
    boost::property_tree::ptree response;

    return response;
  }

  /**
   * @brief order to read object
   *
   * @param properties The properties part of request
   * @param data The data part of request
   * @return The result of request process
   */
  template <typename T, typename std::enable_if<std::is_base_of<
                            root_model_object, T>::value>::type * = nullptr>
  static boost::property_tree::ptree
  readObject(const boost::property_tree::ptree &properties,
             const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;

    return response;
  }

  /**
   * @brief order to delete object
   *
   * @param properties The properties part of request
   * @param data The data part of request
   * @return The result of request process
   */
  template <typename T, typename std::enable_if<std::is_base_of<
                            root_model_object, T>::value>::type * = nullptr>
  static boost::property_tree::ptree
  deleteObject(const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;

    return response;
  }
};

#endif
