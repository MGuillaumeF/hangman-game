#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <memory>   // unique_ptr
#include <thread>
#include <vector>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream
#include <string>  // std::string

#include <boost/property_tree/ptree.hpp>

#include "../../src/TCP/Server.hpp"

BOOST_AUTO_TEST_SUITE(testUserEndpointNominal)

boost::property_tree::ptree sendRequest(const std::string &orderFile) {
  const uint8_t OCTETS_SIZE_MESSAGE = 8;
  const std::string hostname = "127.0.0.1";
  const std::string port = "50000";
  boost::asio::io_context ioContext;

  // connect tcp socket
  boost::asio::ip::tcp::socket socket(ioContext);
  boost::asio::ip::tcp::resolver resolver(ioContext);
  boost::asio::connect(socket, resolver.resolve(hostname, port));

  std::cout << "Connect with server at " << hostname << ":" << port
            << std::endl;

  // read file to send over TCP socket example :
  // "./resources/database-order/create-user.xml"
  const std::ifstream fileToSend(orderFile);
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
  const size_t reply_length = boost::asio::read(
      socket, boost::asio::buffer(reply, OCTETS_SIZE_MESSAGE));
  const std::string messageSizeStr = reply;
  const uint32_t li_hex = std::stoul(messageSizeStr, nullptr, 16);

  char *replyBody = new char[li_hex];
  boost::asio::read(socket, boost::asio::buffer(replyBody, li_hex));

  std::cout << "Size of response is: " << std::endl << li_hex << std::endl;

  std::string cleaned(replyBody);
  delete replyBody;
  cleaned.resize(li_hex);
  std::cout << "Message response is: " << std::endl << cleaned << std::endl;

  return boost::property_tree::ptree();
}

BOOST_AUTO_TEST_CASE(test_create) {

  boost::asio::io_context ioContext;
  const hangman::tcp::Server server(ioContext, 50000);
  std::vector<std::thread> threads;
  threads.reserve(1);

  threads.emplace_back([&ioContext] { ioContext.run(); });

  // open transaction
  // count user as A
  // create user as TOTO
  // count user as B
  // except B == A + 1
  // close transaction

  const boost::property_tree::ptree response =
      sendRequest("./resources/database-order/create-user.xml");
  ioContext.stop();

  for (size_t i = 0; i < threads.size(); ++i) {
    threads[i].join();
  }
}

BOOST_AUTO_TEST_CASE(test_read) {
  // open transaction
  // search user TOTO
  // found TOTO and all attributs
  // close transaction
}

BOOST_AUTO_TEST_CASE(test_update) {
  // open transaction
  // search user TOTO
  // found TOTO and all attributs
  // update email with good pattern
  // close transaction

  // open transaction
  // search user TOTO
  // found TOTO with new email value
  // close transaction
}

BOOST_AUTO_TEST_CASE(test_delete) {
  // open transaction
  // delete TOTO
  // close transaction

  // open transaction
  // count user as C
  // except C == A
  // close transaction
}

BOOST_AUTO_TEST_SUITE_END()
