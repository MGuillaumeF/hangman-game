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

#include "./endpoint/UserDBEndpoint.hpp"

/**
 * @brief Get the Data Base Access object
 *
 * @return odb::core::database
 */
std::shared_ptr<odb::core::database> getDataBaseAccess() {
  char *tempArgv[] = {"_", "--user", "odb_test", "--database", "data.db"};
  int tempArgc = 5;

  return create_database(tempArgc, tempArgv);
}

int32_t main(int argc, char *argv[]) {
  int32_t exitStatus = EXIT_SUCCESS;
  try {
    std::shared_ptr<odb::core::database> db = getDataBaseAccess();
    UserDBEndpoint::getInstance(db);
    boost::asio::io_context ioContext{3};
    const hangman::tcp::Server server(ioContext, 50000);
    ioContext.run();
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    exitStatus = EXIT_FAILURE;
  }
  return exitStatus;
}
