#ifndef __CRUD_ORDER_DISPATCHER_HPP__
#define __CRUD_ORDER_DISPATCHER_HPP__

#include "../model/root_model_object.hxx"
#include "./StatusCode.hpp"
// generated configuration
#include "./DataAccess.hpp"
#include "../config.hpp"

#include <odb/transaction.hxx>

#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <list>
#include <memory>
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
  template <typename T, typename std::enable_if_t<std::is_base_of<
                            root_model_object, T>::value> * = nullptr>
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
    } else {
      response.put("status-code", uint16_t(status_code::METHOD_NOT_ALLOWED));
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
  template <typename T, typename std::enable_if_t<std::is_base_of<
                            root_model_object, T>::value> * = nullptr>
  static boost::property_tree::ptree
  createObject(const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;
    boost::property_tree::ptree errors;
    std::list<T> objects;

    const std::string author = properties.get<std::string>("order-author");
    const boost::optional<const boost::property_tree::ptree &> objectItem =
        data.get_child_optional(T::getObjectType());
    if (objectItem) {
      objects.push_back(*T::parse(*objectItem));

    } else {
      const boost::optional<const boost::property_tree::ptree &> objectList =
          data.get_child_optional(T::getPlurialObjectType());
      if (objectList) {
        for (const auto &objectItem2 : (*objectList)) {
          if (T::getObjectType() == objectItem2.first) {
            objects.push_back(*T::parse(objectItem2.second));
          }
        }
      }
    }
    for (T objectToPersist : objects) {
      auto errorList = objectToPersist.getErrors();
      for (const auto &errorItem : errorList) {
        errors.add_child("error", errorItem.toPtree());
      }
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
    }
    if (errors.empty()) {
      odb::core::transaction t(DataAccess::getDatabaseAccess()->begin());
      for (T objectToPersist : objects) {
        objectToPersist.preCreate(author);
        const uint32_t id =
            DataAccess::getDatabaseAccess()->persist(objectToPersist);

        boost::property_tree::ptree objectId;
        objectId.put("id", id);
        response.add_child(T::getObjectType(), objectId);
      }
      response.put("status-code", uint16_t(status_code::OK));
      t.commit();
    } else {
      response.add_child("errors", errors);
    }
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
  template <typename T, typename std::enable_if_t<std::is_base_of<
                            root_model_object, T>::value> * = nullptr>
  static boost::property_tree::ptree
  updateObject(const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data,
               const bool &full = true) {
    boost::property_tree::ptree response;

    response.put("status-code", uint16_t(status_code::OK));
    return response;
  }

  /**
   * @brief order to read object
   *
   * @param properties The properties part of request
   * @param data The data part of request
   * @return The result of request process
   */
  template <typename T, typename std::enable_if_t<std::is_base_of<
                            root_model_object, T>::value> * = nullptr>
  static boost::property_tree::ptree
  readObject(const boost::property_tree::ptree &properties,
             const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;

    response.put("status-code", uint16_t(status_code::OK));
    return response;
  }

  /**
   * @brief order to delete object
   *
   * @param properties The properties part of request
   * @param data The data part of request
   * @return The result of request process
   */
  template <typename T, typename std::enable_if_t<std::is_base_of<
                            root_model_object, T>::value> * = nullptr>
  static boost::property_tree::ptree
  deleteObject(const boost::property_tree::ptree &properties,
               const boost::property_tree::ptree &data) {
    boost::property_tree::ptree response;
    std::list<uint32_t> objectIds;
    const boost::optional<const boost::property_tree::ptree &> objectItem =
        data.get_child_optional(T::getObjectType());
    if (objectItem) {
      objectIds.push_back((*objectItem).get<uint32_t>("id"));
    } else {
      const boost::optional<const boost::property_tree::ptree &> objectList =
          data.get_child_optional(T::getPlurialObjectType());
      if (objectList) {
        for (const auto &objectItem2 : (*objectList)) {
          if (T::getObjectType() == objectItem2.first) {
            objectIds.push_back(objectItem2.second.get<uint32_t>("id"));
          }
        }
      }
    }
    odb::core::transaction t(DataAccess::getDatabaseAccess()->begin());
    for (const uint32_t objectToDelete : objectIds) {
      DataAccess::getDatabaseAccess()->erase<T>(objectToDelete);
    }
    t.commit();
    response.put("status-code", uint16_t(status_code::OK));
    return response;
  }
};

#endif
