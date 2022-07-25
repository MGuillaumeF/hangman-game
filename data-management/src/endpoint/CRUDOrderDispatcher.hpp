#ifndef __CRUD_ORDER_DISPATCHER_HPP__
#define __CRUD_ORDER_DISPATCHER_HPP__

#include "../model/root_model_object.hxx"

#include <memory>

// generated configuration
#include "./DataAccess.hpp"
#include "config.hpp"
#include <odb/transaction.hxx>

#include <boost/property_tree/ptree.hpp>
#include <iostream>
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

  /**
   * @brief database access pointer of single instance
   *
   */
  static std::shared_ptr<odb::core::database> s_db =
      DataAccess::getDatabaseAccess();

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
  routeObjectType(const T &obj, const std::string &orderType,
                  const boost::property_tree::ptree &properties,
                  const boost::property_tree::ptree &data) {
    // response data property tree
    boost::property_tree::ptree response;

    std::cout << "ORDER TYPE : " << properties.get<std::string>("order-type")
              << std::endl;
    if ("create" == orderType) {
      // create object from data of property tree
      response = createObject<T>(T(), properties, data);
    } else if ("read" == orderType) {
      // read from database, by key of property tree
      response = readObject<T>(T(), properties, data);
    } else if ("update" == orderType) {
      // full update with full replace
      response = updateObject<T>(T(), properties, data);
    } else if ("delete" == orderType) {
      // delete object
      response = deleteObject<T>(T(), properties, data);
    } else if ("patch" == orderType) {
      // partial update of users
      response = updateObject<T>(T(), properties, data, false);
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
  createObject(const T &obj, const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;
    std::list<T> objects;

    const boost::optional<const boost::property_tree::ptree &> objectItem =
        data.get_child_optional(T::object_type);
    if (objectItem) {
      objects.push_back(T::parse(*objectItem));

    } else {
      const boost::optional<const boost::property_tree::ptree &> objectList =
          data.get_child_optional(T::plurial_object_type);
      if (objectList) {
        for (const auto &objectItem2 : (*objectList)) {
          if (T::object_type == objectItem2.first) {
            objects.push_back(T::parse(objectItem2.second));
          }
        }
      }
    }
    odb::core::transaction t(s_db->begin());
    for (T objectToPersist : objects) {
      if (0 != objectToPersist.getId()) {
        std::cerr
            << "[WARNNING] create content ignored : the id must be 0, value "
            << objectToPersist.getId() << " ignored" << std::endl;
      }
      if (0 != objectToPersist.getVersion()) {
        std::cerr << "[WARNNING] create content ignored : the version will be "
                     "overrided to 1, value "
                  << objectToPersist.getVersion() << " ignored" << std::endl;
      }
      objectToPersist.setVersion(1);
      const uint32_t id = s_db->persist(objectToPersist);

      boost::property_tree::ptree objectId;
      objectId.put("id", id);
      response.add_child(T::object_type, objectId);
    }
    t.commit();
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
  updateObject(const T &obj, const boost::property_tree::ptree &properties,
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
  readObject(const T &obj, const boost::property_tree::ptree &properties,
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
  deleteObject(const T &obj, const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;

    return response;
  }
};

#endif
