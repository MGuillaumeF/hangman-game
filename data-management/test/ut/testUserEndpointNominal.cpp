#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <exception>
#include <iostream> // std::cout
#include <thread>
#include <vector>

#include <string> // std::string

#include <boost/property_tree/ptree.hpp>

#include "../../src/TCP/Client.hpp"
#include "../../src/TCP/Server.hpp"

BOOST_AUTO_TEST_SUITE(testUserEndpointNominal)

BOOST_AUTO_TEST_CASE(test_create_one) {

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

  uint8_t counter = 0;

  const boost::property_tree::ptree response =
      hangman::tcp::Client::sendRequest(
          "127.0.0.1", 50000, "./resources/database-order/create-user.xml");
  ioContext.stop();

  for (size_t i = 0; i < threads.size(); ++i) {
    threads[i].join();
  }

  for (const auto &[nodeName, nodeContent] : response) {
    if (nodeName == "user") {
      counter += 1;
    }
  }
}

BOOST_AUTO_TEST_CASE(test_create_many) {

  boost::asio::io_context ioContext;
  const hangman::tcp::Server server(ioContext, 50000);
  std::vector<std::thread> threads;
  threads.reserve(1);

  threads.emplace_back([&ioContext] { ioContext.run(); });

  // open transaction
  // count user as A
  // create user as User1 and User2
  // count user as B
  // except B == A + 2
  // close transaction

  const boost::property_tree::ptree response =
      hangman::tcp::Client::sendRequest(
          "127.0.0.1", 50000, "./resources/database-order/create-users.xml");
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
