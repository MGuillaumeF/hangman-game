#include "./DataAccess.hpp"

// init instance as nullptr before first init
DataAccess *DataAccess::s_instance = nullptr;

/**
 * @brief Construct a new Data Access:: Data Access object
 *
 */
DataAccess::DataAccess() {
  char *tempArgv[] = {"_", "--user", "odb_test", "--database", "data.db"};
  int tempArgc = 5;

  m_db = create_database(tempArgc, tempArgv);
};

/**
 * @brief Get the Data Base object
 *
 * @return std::unique_ptr<odb::core::database> The database access pointer
 */
std::shared_ptr<odb::core::database> DataAccess::getDataBase() { return m_db; }

/**
 * @brief Get the Database Access object
 *
 * @return std::shared_ptr<odb::core::database> The database access pointer
 */
std::shared_ptr<odb::core::database> DataAccess::getDatabaseAccess() {
  if (nullptr == s_instance) {
    s_instance = new DataAccess();
  }
  return s_instance->getDataBase();
}
