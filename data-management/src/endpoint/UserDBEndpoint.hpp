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

class UserDBEndpoint {
public:
  /**
   * @brief Construct a new User DB Endpoint object
   *
   */
  UserDBEndpoint() = delete;
  /**
   * @brief Destroy the User DB Endpoint object
   *
   */
  ~UserDBEndpoint() = delete;

  /**
   * @brief Create a User object
   *
   * @param data The property tree data of user to create
   * @return uint32_t The user id after create
   */
  static uint32_t createUser(const std::unique_ptr<odb::core::database> &db,
                             const boost::property_tree::ptree &data);

  /**
   * @brief function to delete user by id
   *
   * @param db The database access
   * @param id The id of user t delete
   */
  static void deleteUser(const std::unique_ptr<odb::core::database> &db,
                         const uint32_t &id);

  /**
   * @brief function to connect user by login and password
   *
   * @param db The database access
   * @param data The property tree data of user to connect
   * @return std::string The new token of connected user
   */
  static std::string connectUser(const std::unique_ptr<odb::core::database> &db,
                                 const boost::property_tree::ptree &data);
};

#endif