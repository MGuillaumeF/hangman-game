#include "../TCP/Client.hpp"
#include <exception>
#include <fstream>
#include <iostream> // std::cout

#include <boost/asio.hpp>

/**
 * @brief This entry point is Client tcp application, to send tcp xml file to tcp server with command
 */ 
int32_t main(int argc, char *argv[]) {
  // by default exit code is success
  int32_t exitCode = EXIT_SUCCESS;
  // command must have 3 arguments (+1 for process name at 0 index)
  const uint8_t ARGS_SIZE_CLI = 4;
  // first argument of command must be hostname of server to join
  const uint8_t ARG_CLI_HOSTNAME_INDEX = 1;
  // second argument of command must be the port of server to join
  const uint8_t ARG_CLI_PORT_INDEX = 2;
  // third argument of command is path to xml file to send
  const uint8_t ARG_FILEPATH_TO_SEND_INDEX = 3;
  // if number of arguments is valid try to send xml file
  if (argc == ARGS_SIZE_CLI) {
    try {
      // get hostname of arg list
      const std::string hostname = argv[ARG_CLI_HOSTNAME_INDEX];
      // get port of arg list
      const uint16_t port = std::strtoul(argv[ARG_CLI_PORT_INDEX], nullptr, 10);

      // read file to send over TCP socket example :
      // "./resources/database-order/create-user.xml"

      // send tcp request over socket
      hangman::tcp::Client::sendRequest(hostname, port,
                                        argv[ARG_FILEPATH_TO_SEND_INDEX]);
    } catch (const std::exception &e) {
      // if error status code of application change to failure and log exception
      exitCode = EXIT_FAILURE;
      std::cerr << "Exception: " << e.what() << std::endl;
    }
  } else {
    // if arguments number invalid log explaination 
    std::cerr << "bad use of tcp client, use it with : " << std::endl
              << "client.exe hostname port file_to_send.txt" << std::endl;
    exitCode = EXIT_FAILURE;
  }
  return exitCode;
}
