#include "../src/TCP/Client.hpp"
#include <exception>
#include <fstream>
#include <iostream> // std::cout

#include <boost/asio.hpp>

int32_t main(int argc, char *argv[]) {
  int32_t exitCode = EXIT_SUCCESS;
  const uint8_t ARGS_SIZE_CLI = 4;
  const uint8_t ARG_CLI_HOSTNAME_INDEX = 1;
  const uint8_t ARG_CLI_PORT_INDEX = 2;
  const uint8_t ARG_FILEPATH_TO_SEND_INDEX = 3;
  if (argc == ARGS_SIZE_CLI) {

    try {
      const std::string hostname = argv[ARG_CLI_HOSTNAME_INDEX];
      const uint16_t port = std::strtoul(argv[ARG_CLI_PORT_INDEX], nullptr);

      // read file to send over TCP socket example :
      // "./resources/database-order/create-user.xml"

      // send tcp request over socket
      Client::sendRequest(hostname, , argv[ARG_FILEPATH_TO_SEND_INDEX]);
    } catch (const std::exception &e) {
      std::cerr << "Exception: " << e.what() << std::endl;
    }
  } else {
    std::cerr << "bad use of tcp client, use it with : " << std::endl
              << "client.exe hostname port file_to_send.txt" << std::endl;
    exitCode = EXIT_FAILURE;
  }
  return exitCode;
}
