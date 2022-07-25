#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <iostream>

#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <memory>   // shared_ptr
#include <thread>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream
#include <string>  // std::string

#include <odb/transaction.hxx>

#include "../../src/connector/database.hxx" // create database access
#include "../../src/endpoint/CRUDOrderDispatcher.hpp"
#include "../../src/endpoint/DataAccess.hpp"
#include "../../src/endpoint/UserDBEndpoint.hpp"
#include <boost/property_tree/ptree.hpp>

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

BOOST_AUTO_TEST_SUITE(testDatabasePersistency)

std::size_t printUserCount(std::shared_ptr<odb::core::database> db) {
  odb::core::transaction t(db->begin());

  // The result of this (aggregate) query always has exactly one element
  // so use the query_value() shortcut.
  //
  const user_stat ps(db->query_value<user_stat>());
  const std::size_t size = ps.count;
  std::cout << std::endl << "count  : " << size << std::endl;

  t.commit();

  return size;
}

BOOST_AUTO_TEST_CASE(test_create) {

  std::shared_ptr<odb::core::database> db = DataAccess::getDatabaseAccess();

  BOOST_CHECK_EQUAL(0, printUserCount(db));

  uint32_t john_id = -1;
  uint32_t joe_id = -1;

  // Create a few persistent user objects.
  //
  user john;
  john.setLogin("John");
  john.setPassword("password_1");
  john.setSaltUser("salt_user_1");
  john.setSaltSession("salt_session_1");
  john.setToken("token_1");

  user jane;
  jane.setLogin("Jane");
  jane.setPassword("password_2");
  jane.setSaltUser("salt_user_2");
  jane.setSaltSession("salt_session_2");
  jane.setToken("token_2");

  user joe;
  joe.setLogin("Joe");
  joe.setPassword("password_3");
  joe.setSaltUser("salt_user_3");
  joe.setSaltSession("salt_session_3");
  joe.setToken("token_3");

  user frank;
  frank.setLogin("Frank");
  frank.setPassword("password_4");
  frank.setSaltUser("salt_user_4");
  frank.setSaltSession("salt_session_4");
  frank.setToken("token_4");

  // Make objects persistent and save their ids for later use.
  //
  {
    odb::core::transaction t(db->begin());

    john_id = db->persist(john);
    db->persist(jane);
    joe_id = db->persist(joe);
    db->persist(frank);

    t.commit();
  }

  // Say hello to those have id under 2.
  //
  {
    odb::core::transaction t(db->begin());

    odb::result<user> r(db->query<user>(odb::query<user>::id < 10));

    for (odb::result<user>::iterator i(r.begin()); i != r.end(); ++i) {
      std::cout << "Hello, " << i->getLogin() << " " << i->getPassword() << "!"
                << std::endl;
    }

    t.commit();
  }

  // Joe is logged, so update his token.
  //
  {
    odb::core::transaction t(db->begin());

    std::unique_ptr<user> joe(db->load<user>(joe_id));
    joe->setToken("new token");
    db->update(*joe);

    t.commit();
  }

  // Alternative implementation without using the id.
  //
  // Here we know that there can be only one Joe  in our
  // database so we use the query_one() shortcut instead of
  // manually iterating over the result returned by query().
  //
  boost::property_tree::ptree frk;
  frk.put("login", "Frank");
  frk.put("password", "password_4");
  const std::string tok = UserDBEndpoint::getInstance()->connectUser(frk);

  std::cout << "New token found is \"" << tok << "\"" << std::endl;

  BOOST_CHECK_EQUAL(4, printUserCount(db));

  // John Doe is no longer in our database.
  boost::property_tree::ptree del;
  del.put("user.id", john_id);
  CRUDOrderDispatcher::deleteObject<user>(boost::property_tree::ptree(), del);

  BOOST_CHECK_EQUAL(3, printUserCount(db));
}

BOOST_AUTO_TEST_SUITE_END()
