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
#include "../../src/model/mysql/group-odb.hxx"
#include "../../src/model/mysql/group.hxx"
#include "../../src/model/mysql/user-odb.hxx"
#include "../../src/model/mysql/user.hxx"
#elif defined(DATABASE_SQLITE)
#include "../../src/model/sqlite/group-odb.hxx"
#include "../../src/model/sqlite/group.hxx"
#include "../../src/model/sqlite/user-odb.hxx"
#include "../../src/model/sqlite/user.hxx"
#elif defined(DATABASE_PGSQL)
#include "../../src/model/pgsql/group-odb.hxx"
#include "../../src/model/pgsql/group.hxx"
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
  uint32_t jane_id = -1;
  uint32_t user_group_id = -1;

  group userGroup;
  userGroup.setName("User");
  std::vector<std::shared_ptr<group>> groups;
  groups.push_back(userGroup);

  // Create a few persistent user objects.
  //
  user john;
  john.setLogin("John");
  john.setPassword("password_1");
  john.setSaltUser("salt_user_1");
  john.setSaltSession("salt_session_1");
  john.setToken("token_1");
  john.setGroups(groups);

  user jane;
  jane.setLogin("Jane");
  jane.setPassword("password_2");
  jane.setSaltUser("salt_user_2");
  jane.setSaltSession("salt_session_2");
  jane.setToken("token_2");
  jane.setGroups(groups);

  user joe;
  joe.setLogin("Joe");
  joe.setPassword("password_3");
  joe.setSaltUser("salt_user_3");
  joe.setSaltSession("salt_session_3");
  joe.setToken("token_3");
  joe.setGroups(groups);

  user frank;
  frank.setLogin("Frank");
  frank.setPassword("password_4");
  frank.setSaltUser("salt_user_4");
  frank.setSaltSession("salt_session_4");
  frank.setToken("token_4");
  frank.setGroups(groups);

  // Make objects persistent and save their ids for later use.
  //
  {
    odb::core::transaction t(db->begin());

    user_group_id = db->persist(userGroup);

    john_id = db->persist(john);
    jane_id = db->persist(jane);
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

  // Joe and Jane are friends
  //
  {
    odb::core::transaction t(db->begin());
    std::vector<std::shared_ptr<user>> friends;
    std::shared_ptr<user> joe(db->load<user>(joe_id));
    std::shared_ptr<user> jane(db->load<user>(jane_id));
    friends.push_back(jane);
    joe->setFriends(friends);
    db->update(*joe);
    t.commit();
  }

  // Joe and Jane are friends check
  //
  {
    odb::core::transaction t(db->begin());
    std::vector<std::shared_ptr<user>> friends;
    std::unique_ptr<user> joe(db->load<user>(joe_id));
    std::unique_ptr<user> jane(db->load<user>(jane_id));
    friends = joe->getFriends();
    std::cout << "Joe has " << friends.size() << " friends" << std::endl;
    BOOST_CHECK_EQUAL(1, friends.size());
    if (friends.size() > 0) {
      BOOST_CHECK_EQUAL("Jane", friends.front()->getLogin());
      BOOST_CHECK_EQUAL(0, friends.front()->getFriends().size());
    }
    t.commit();
  }

  // Joe and Jane are group check
  //
  {
    odb::core::transaction t(db->begin());
    std::vector<std::shared_ptr<group>> joeGroups;
    std::vector<std::shared_ptr<group>> janeGroups;
    std::vector<std::weak_ptr<user>> userGroupUsers;
    std::unique_ptr<group> userGroup(db->load<group>(user_group_id));
    std::unique_ptr<user> joe(db->load<user>(joe_id));
    std::unique_ptr<user> jane(db->load<user>(jane_id));
    joeGroups = joe->getGroups();
    janeGroups = jane->getGroups();
    std::cout << "Joe has " << joeGroups.size() << " groups" << std::endl;
    std::cout << "Jane has " << janeGroups.size() << " groups" << std::endl;

    BOOST_CHECK_EQUAL(1, joeGroups.size());
    if (joeGroups.size() > 0) {
      BOOST_CHECK_EQUAL("User", joeGroups.front()->getName());
      // BOOST_CHECK_EQUAL(2, joeGroups.front()->getMembers().size());
    }
    BOOST_CHECK_EQUAL(1, janeGroups.size());
    if (janeGroups.size() > 0) {
      BOOST_CHECK_EQUAL("User", janeGroups.front()->getName());
      // BOOST_CHECK_EQUAL(2, janeGroups.front()->getMembers().size());
    }
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
