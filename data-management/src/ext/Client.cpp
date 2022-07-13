#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <memory>   // unique_ptr
#include <thread>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream
#include <string>  // std::string

#include <boost/asio.hpp>

int32_t main(int argc, char *argv[]) {
  int32_t exitCode = EXIT_SUCCESS;
  // 8 first octets are allocated to the size of file sended to manage buffer
  // size
  const uint8_t OCTETS_SIZE_MESSAGE = 8;
  const uint8_t ARGS_SIZE_CLI = 4;
  const uint8_t ARG_CLI_HOSTNAME_INDEX = 1;
  const uint8_t ARG_CLI_PORT_INDEX = 2;
  const uint8_t ARG_FILEPATH_TO_SEND_INDEX = 3;
  if (argc == ARGS_SIZE_CLI) {

    try {
      const std::string hostname = argv[ARG_CLI_HOSTNAME_INDEX];
      const std::string port = argv[ARG_CLI_PORT_INDEX];
      boost::asio::io_context ioContext;

      // connect tcp socket
      boost::asio::ip::tcp::socket socket(ioContext);
      boost::asio::ip::tcp::resolver resolver(ioContext);
      boost::asio::connect(socket, resolver.resolve(hostname, port));
      std::cout << "Connect with server at " << hostname << ":" << port
                << std::endl;

      // read file to send over TCP socket example :
      // "./resources/database-order/create-user.xml"
      const std::ifstream fileToSend(argv[ARG_FILEPATH_TO_SEND_INDEX]);
      std::stringstream fileToSendStream;
      fileToSendStream << fileToSend.rdbuf();
      const std::string request = fileToSendStream.str();

      // format message
      std::stringstream stream;
      stream << std::setfill('0') << std::setw(OCTETS_SIZE_MESSAGE);
      stream << std::hex << request.size();
      stream << std::hex << request;

      // send message
      std::cout << "Message send : " << std::endl << stream.str() << std::endl;
      boost::asio::write(
          socket, boost::asio::buffer(stream.str().c_str(),
                                      request.size() + OCTETS_SIZE_MESSAGE));

      char reply[OCTETS_SIZE_MESSAGE];
      boost::asio::read(socket,
                        boost::asio::buffer(reply, OCTETS_SIZE_MESSAGE));
      const std::string messageSizeStr = reply;
      const uint32_t li_hex = std::stoul(messageSizeStr, nullptr, 16);

      auto replyBody = new char[li_hex];
      boost::asio::read(socket, boost::asio::buffer(replyBody, li_hex));

      std::cout << "Size of response is: " << std::endl << li_hex << std::endl;

      std::string cleaned(replyBody);
      delete[] replyBody;
      cleaned.resize(li_hex);
      std::cout << "Message response is: " << std::endl << cleaned << std::endl;
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
