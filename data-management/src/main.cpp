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

// create database access
#include "./endpoint/DataAccess.hpp"
#include "./endpoint/UserDBEndpoint.hpp"

#include "./TCP/Server.hpp"

int32_t main(int argc, char *argv[]) {
  int32_t exitStatus = EXIT_SUCCESS;
  try {
    std::shared_ptr<odb::core::database> db = DataAccess::getDatabaseAccess();
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
