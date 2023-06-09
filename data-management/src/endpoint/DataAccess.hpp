#ifndef __DATA_ACCESS_HPP__
#define __DATA_ACCESS_HPP__

#include <memory>

// generated configuration
#include "../config.hpp"

#include <odb/transaction.hxx>

// create database access
#include "../connector/database.hxx"

/**
 * @brief class to access database
 *
 */
class DataAccess {
  /**
   * @brief static instance of singleton to have unique DataAccess instance
   *
   */
  static std::unique_ptr<DataAccess> s_instance;
  /**
   * @brief The database pointer access is unique for the instance
   *
   */
  std::shared_ptr<odb::core::database> m_db;

public:
  /**
   * @brief Construct a new Data Access object
   *
   */
  explicit DataAccess();

  /**
   * @brief Get the Database Access object
   *
   * @return odb::core::database* The database access pointer
   */
  static std::shared_ptr<odb::core::database> getDatabaseAccess();
  /**
   * @brief Get the Data Base object
   *
   * @return std::shared_ptr<odb::core::database> The database access pointer
   */
  std::shared_ptr<odb::core::database> getDataBase() const;
  /**
   * @brief Destroy the Data Access object
   *
   */
  ~DataAccess() = default;
};

#endif
