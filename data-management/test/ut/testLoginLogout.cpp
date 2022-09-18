#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../../src/endpoint/DataAccess.hpp"
#include <odb/transaction.hxx>

#if defined(DATABASE_MYSQL)
#include "../../src/model/mysql/user-odb.hxx"
#include "../../src/model/mysql/user.hxx"
#elif defined(DATABASE_SQLITE)
#include "../../src/model/sqlite/user-odb.hxx"
#include "../../src/model/sqlite/user.hxx"
#elif defined(DATABASE_PGSQL)
#include "../../src/model/pgsql/user-odb.hxx"
#include "../../src/model/pgsql/user.hxx"
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include <cstdint>
#include <exception>
#include <iostream> // std::cout
#include <memory>   // shared_ptr
#include <thread>
#include <vector>

#include <string> // std::string

#include <boost/property_tree/ptree.hpp>

#include "../../src/TCP/Client.hpp"
#include "../../src/TCP/Server.hpp"

BOOST_AUTO_TEST_SUITE(testLoginLogout)

BOOST_AUTO_TEST_CASE(test_nominal) {
  std::shared_ptr<odb::core::database> db = DataAccess::getDatabaseAccess();

  boost::asio::io_context ioContext;
  const hangman::tcp::Server server(ioContext, 50000);
  std::vector<std::thread> threads;
  threads.reserve(1);

  threads.emplace_back([&ioContext] { ioContext.run(); });

  // create the user to test logging
  const boost::property_tree::ptree responseCreate =
      hangman::tcp::Client::sendRequest(
          "127.0.0.1", 50000,
          "./resources/database-order/login-logout/create-login-user.xml");

  BOOST_CHECK_EQUAL(200, responseCreate.get<uint16_t>("status-code"));

  // send logging request
  const boost::property_tree::ptree responseLogin =
      hangman::tcp::Client::sendRequest(
          "127.0.0.1", 50000,
          "./resources/database-order/login-logout/login-user.xml");
  BOOST_CHECK_EQUAL(200, responseLogin.get<uint16_t>("status-code"));

  // count if user is created
  // force update of token
  uint8_t counter = 0;
  for (const auto &node : responseCreate) {
    if (node.first == "user") {
      counter += 1;
      const uint32_t id = node.second.get<uint32_t>("id");

      odb::core::transaction t(db->begin());

      std::shared_ptr<user> loggedUser(db->load<user>(id));
      loggedUser->setToken("example_token");
      db->update(*loggedUser);

      t.commit();
    }
  }
  BOOST_CHECK_EQUAL(1, counter);

  // logout of user
  const boost::property_tree::ptree responseLogout =
      hangman::tcp::Client::sendRequest(
          "127.0.0.1", 50000,
          "./resources/database-order/login-logout/logout-user.xml");
  BOOST_CHECK_EQUAL(200, responseLogout.get<uint16_t>("status-code"));

  ioContext.stop();

  for (size_t i = 0; i < threads.size(); ++i) {
    threads[i].join();
  }
}

BOOST_AUTO_TEST_SUITE_END()
