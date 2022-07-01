#ifndef __USER_DB_ENDPOINT_HPP__
#define __USER_DB_ENDPOINT_HPP__

#include <memory>
#include <string>

#include "./DataAccess.hpp"

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
  std::shared_ptr<odb::core::database> m_db = DataAccess::getDatabaseAccess();

  /**
   * @brief user endpoint pointer of single instance
   *
   */
  static std::unique_ptr<UserDBEndpoint> s_instance;
  /**
   * @brief Construct a new User DB Endpoint object
   *
   */
  explicit UserDBEndpoint() = default;

public:
  /**
   * @brief Destroy the User DB Endpoint object
   *
   */
  ~UserDBEndpoint() = default;

  /**
   * @brief methode to get unique instance of user endpoint
   *
   * @return UserDBEndpoint*
   */
  static std::unique_ptr<UserDBEndpoint> &getInstance();

  /**
   * @brief Create User(s) object(s)
   *
   * @param data The property tree data of user(s) to create
   * @return boost::property_tree::ptree The user(s) id(s) list after create
   */
  boost::property_tree::ptree
  createUser(const boost::property_tree::ptree &data) const;

  /**
   * @brief function to delete user by id
   *
   * @param data The property tree data of user(s) to delete
   */
  void deleteUser(const boost::property_tree::ptree &data) const;

  /**
   * @brief function to connect user by login and password
   *
   * @param data The property tree data of user to connect
   * @return std::string The new token of connected user
   */
  std::string connectUser(const boost::property_tree::ptree &data) const;

  /**
   * @brief method to parse user from property tree
   *
   * @param data The boost proerty tree of user
   * @return user User object
   */
  static user parse(const boost::property_tree::ptree &data);
};

#endif
