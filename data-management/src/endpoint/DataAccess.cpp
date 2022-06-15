#include "./DataAccess.hpp"
DataAccess *DataAccess::s_instance = nullptr;

DataAccess::DataAccess() {
  char *tempArgv[] = {"_", "--user", "odb_test", "--database", "data.db"};
  int tempArgc = 5;

  m_db = create_database(tempArgc, tempArgv);
};

odb::core::database *DataAccess::getDataBase() { return m_db; }

odb::core::database *DataAccess::getDatabaseAccess() {
  if (s_instance == nullptr) {
    s_instance = new DataAccess();
  }
  return s_instance->getDataBase();
}