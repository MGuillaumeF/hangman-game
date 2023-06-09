#ifndef __USER_DB_ENDPOINT_HPP__
#define __USER_DB_ENDPOINT_HPP__

#include <memory>
#include <string>

#include "./DataAccess.hpp"

// generated configuration
#include "../config.hpp"

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
#include "../model/pgsql/user.hxx"
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
   * @brief user connected to the connetor
   *
   */
  std::set<std::string> m_connectedUsers;

  /**
   * @brief user endpoint pointer of single instance
   *
   */
  static std::unique_ptr<UserDBEndpoint> s_instance;

public:
  /**
   * @brief Construct a new User DB Endpoint object
   *
   */
  explicit UserDBEndpoint() = default;

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
   * @brief function to connect user by login and password
   *
   * @param properties The properties part of request
   * @param data The property tree data of user to connect
   * @return The result of request process
   */
  boost::property_tree::ptree
  connectUser(const boost::property_tree::ptree &properties,
              const boost::property_tree::ptree &data);

  /**
   * @brief function to disconnect user by login and token
   *
   * @param properties The properties part of request
   * @param data The property tree data of user to disconnect
   * @return The result of request process
   */
  boost::property_tree::ptree
  disconnectUser(const boost::property_tree::ptree &properties,
                 const boost::property_tree::ptree &data);
};

#endif
