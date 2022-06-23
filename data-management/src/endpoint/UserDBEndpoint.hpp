#ifndef __USER_DB_ENDPOINT_HPP__
#define __USER_DB_ENDPOINT_HPP__

#include <memory>
#include <string>

// generated configuration
#include "config.hpp"

#include <boost/property_tree/ptree.hpp>
#include <odb/transaction.hxx>

// create database access
#include "../connector/database.hxx"

#if defined(DATABASE_MYSQL)
#include "../model/mysql/user-odb.hxx"
#include "../model/mysql/user.hxx"
#elif defined(DATABASE_SQLITE)
#include "../model/sqlite/user-odb.hxx"
#include "../model/sqlite/user.hxx"
#elif defined(DATABASE_PGSQL)
#include "../model/pgsql/user-odb.hxx"
#include ".;/model/pgsql/user.hxx"
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif
/**
 * @brief class of user endpoint management
 *
 */
class UserDBEndpoint {
  /**
   * @brief database access pointer of single instance
   *
   */
  std::shared_ptr<odb::core::database> m_db;

  /**
   * @brief user endpoint pointer of single instance
   *
   */
  static UserDBEndpoint *s_instance;
  /**
   * @brief Construct a new User DB Endpoint object
   *
   */
  explicit UserDBEndpoint(std::shared_ptr<odb::core::database> db);

public:
  /**
   * @brief Destroy the User DB Endpoint object
   *
   */
  ~UserDBEndpoint() = delete;

  /**
   * @brief methode to get unique instance of user endpoint
   *
   * @param db The database access pointer
   * @return UserDBEndpoint*
   */
  static UserDBEndpoint *
  getInstance(std::shared_ptr<odb::core::database> db = nullptr);

  /**
   * @brief Create a User object
   *
   * @param data The property tree data of user to create
   * @return uint32_t The user id after create
   */
  uint32_t createUser(const boost::property_tree::ptree &data) const;

  /**
   * @brief function to delete user by id
   *
   * @param id The id of user t delete
   */
  void deleteUser(const uint32_t &id) const;

  /**
   * @brief function to connect user by login and password
   *
   * @param data The property tree data of user to connect
   * @return std::string The new token of connected user
   */
  std::string connectUser(const boost::property_tree::ptree &data) const;
};

#endif
