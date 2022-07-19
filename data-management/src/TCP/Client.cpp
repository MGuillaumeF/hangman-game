#include "./Client.hpp"

#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <vector>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream

#include <boost/asio.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace hangman {
namespace tcp {

/**
 * method to send file with TCP Request to the data manager
 * @param hostname The hostname of data manager to join
 * @param port The port of data manager to join
 * @param filename The file to send over TCP request
 * @return The data manager response
 */
boost::property_tree::ptree Client::sendRequest(const std::string &hostname,
                                                const uint16_t &port,
                                                const std::string &filename) {
  const uint8_t OCTETS_SIZE_MESSAGE = 8;
  boost::asio::io_context ioContext;

  // connect tcp socket
  boost::asio::ip::tcp::socket socket(ioContext);
  boost::asio::ip::tcp::resolver resolver(ioContext);
  boost::asio::connect(
      socket, resolver.resolve(hostname, std::to_string(port).c_str()));

  std::cout << "Connect with server at " << hostname << ":" << port
            << std::endl;

  // read file to send over TCP socket example :
  // "./resources/database-order/create-user.xml"
  const std::ifstream fileToSend(filename);
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
  boost::asio::write(socket,
                     boost::asio::buffer(stream.str().c_str(),
                                         request.size() + OCTETS_SIZE_MESSAGE));

  char reply[OCTETS_SIZE_MESSAGE];
  boost::asio::read(socket, boost::asio::buffer(reply, OCTETS_SIZE_MESSAGE));
  const std::string messageSizeStr = reply;
  const uint32_t li_hex = std::stoul(messageSizeStr, nullptr, 16);

  char *replyBody = new char[li_hex];
  boost::asio::read(socket, boost::asio::buffer(replyBody, li_hex));

  std::cout << "Size of response is: " << std::endl << li_hex << std::endl;

  std::string cleaned(replyBody);
  delete[] replyBody;
  cleaned.resize(li_hex);
  std::cout << "Message response is: " << std::endl << cleaned << std::endl;

  boost::property_tree::ptree response;

  std::stringstream responseStream;
  responseStream << cleaned;

  boost::property_tree::xml_parser::read_xml(responseStream, response);

  return response;
}

} // namespace tcp
} // namespace hangman
