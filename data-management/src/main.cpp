#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <memory>   // unique_ptr
#include <thread>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream
#include <string>  // std::string

#include <boost/property_tree/ptree.hpp>
#include <odb/transaction.hxx>

// create database access
#include "./connector/database.hxx"
#include "./endpoint/UserDBEndpoint.hpp"

#if defined(DATABASE_MYSQL)
#include "./model/mysql/user-odb.hxx"
#include "./model/mysql/user.hxx"
#elif defined(DATABASE_SQLITE)
#include "./model/sqlite/user-odb.hxx"
#include "./model/sqlite/user.hxx"
#elif defined(DATABASE_PGSQL)
#include "./model/pgsql/user-odb.hxx"
#include "./model/pgsql/user.hxx"
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include "./TCP/Server.hpp"

enum { max_length = 1024 };

/**
 * @brief Get the Data Base Access object
 *
 * @return std::unique_ptr<odb::core::database>
 */
std::unique_ptr<odb::core::database> getDataBaseAccess() {
  char *exec_name = "./HangmanGameTest";
  char *user_key = "--user";
  char *user_value = "odb_test";
  char *database_key = "--database";
  char *database_value = "odb_test";
  char *tempArgv[] = {exec_name, user_key, user_value, database_key,
                      database_value};
  int tempArgc = 5;

  return create_database(tempArgc, tempArgv);
}

void printUserCount(const std::unique_ptr<odb::core::database> &db) {
  odb::core::transaction t(db->begin());

  // The result of this (aggregate) query always has exactly one element
  // so use the query_value() shortcut.
  //
  const user_stat ps(db->query_value<user_stat>());

  std::cout << std::endl << "count  : " << ps.count << std::endl;

  t.commit();
}

int32_t main(int argc, char *argv[]) {
  int32_t exitStatus = EXIT_SUCCESS;
  try {
    boost::asio::io_context ioContext{3};
    const hangman::tcp::Server server(ioContext, 50000);
    ioContext.run();
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    exitStatus = EXIT_FAILURE;
  }
  return exitStatus;
}
