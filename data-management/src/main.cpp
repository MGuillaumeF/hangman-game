#include <exception>
#include <iostream>        // std::cout
#include <odb/session.hxx> // session for all transactions

// import TCP Server
#include "./TCP/Server.hpp"
#include "./config.hpp"

int32_t main(int argc, char *argv[]) {
  odb::session s;
  // by default process exit status is success
  int32_t exitStatus = EXIT_SUCCESS;
  try {

    std::cout << PROJECT_NAME << " - " << APPLICATION_NAME << " - version "
              << PROJECT_VERSION;
    // create io context for server
    boost::asio::io_context ioContext{3};
    // attach tcp server on io context and declare it on 50000 port
    const hangman::tcp::Server server(ioContext, 50000);
    // start tcp server
    ioContext.run();
  } catch (const std::exception &e) {
    // if error raised log it and exit code is changed to failure
    std::cerr << "Exception: " << e.what() << "\n";
    exitStatus = EXIT_FAILURE;
  }
  return exitStatus;
}
