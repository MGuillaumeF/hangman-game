#include "./DataAccess.hpp"
DataAccess *DataAccess::s_instance = nullptr;

DataAccess::DataAccess() {
  char *tempArgv[] = {"_", "--user", "odb_test", "--database", "data.db"};
  uint16_t tempArgc = 5;

  m_db = create_database(tempArgc, tempArgv);
};

odb::core::database *DataAccess::getDataBase() { return m_db; }

odb::core::database *DataAccess::getDatabaseAccess() {
  if (nullptr == s_instance) {
    s_instance = new DataAccess();
  }
  return s_instance->getDataBase();
}
