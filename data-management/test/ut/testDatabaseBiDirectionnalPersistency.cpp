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

#include <odb/session.hxx>
#include <odb/transaction.hxx>

#include "../../src/connector/database.hxx" // create database access
#include "../../src/endpoint/CRUDOrderDispatcher.hpp"
#include "../../src/endpoint/DataAccess.hpp"
#include "../../src/endpoint/UserDBEndpoint.hpp"
#include <boost/property_tree/ptree.hpp>

#if defined(DATABASE_MYSQL)
#include "../../src/model/mysql/dictionary-odb.hxx"
#include "../../src/model/mysql/dictionary.hxx"
#include "../../src/model/mysql/word-odb.hxx"
#include "../../src/model/mysql/word.hxx"
#elif defined(DATABASE_SQLITE)
#include "../../src/model/sqlite/dictionary-odb.hxx"
#include "../../src/model/sqlite/dictionary.hxx"
#include "../../src/model/sqlite/word-odb.hxx"
#include "../../src/model/sqlite/word.hxx"
#elif defined(DATABASE_PGSQL)
#include "../../src/model/pgsql/dictionary-odb.hxx"
#include "../../src/model/pgsql/dictionary.hxx"
#include "../../src/model/pgsql/word-odb.hxx"
#include "../../src/model/pgsql/word.hxx"
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

BOOST_AUTO_TEST_SUITE(testDatabaseBiDirectionnalPersistency)

template <typename data_stat>
std::size_t printDataCount(std::shared_ptr<odb::core::database> db) {
  odb::core::transaction t(db->begin());

  // The result of this (aggregate) query always has exactly one element
  // so use the query_value() shortcut.
  //
  const data_stat ps(db->query_value<data_stat>());
  const std::size_t size = ps.count;
  std::cout << std::endl << "count  : " << size << std::endl;

  t.commit();

  return size;
}

BOOST_AUTO_TEST_CASE(test_create) {

  odb::session s;
  std::shared_ptr<odb::core::database> db = DataAccess::getDatabaseAccess();

  BOOST_CHECK_EQUAL(0, printDataCount<word_stat>(db));
  BOOST_CHECK_EQUAL(0, printDataCount<dictionary_stat>(db));

  uint32_t hello_id = -1;
  uint32_t world_id = -1;
  uint32_t english_id = -1;

  std::shared_ptr<dictionary> englishDictionary =
      std::make_shared<dictionary>();
  englishDictionary->setName("ENGLISH");
  englishDictionary->setCountryCode("EN");

  // Create a few persistent word objects.
  //
  std::shared_ptr<word> hello = std::make_shared<word>();
  hello->setName("hello");
  std::vector<std::string> helloDefinitions;
  helloDefinitions.emplace_back(std::string("word of salutation"));
  hello->setDefinitions(helloDefinitions);
  hello->setDictionary(englishDictionary);

  std::shared_ptr<word> world = std::make_shared<word>();
  world->setName("world");
  std::vector<std::string> worldDefinitions;
  worldDefinitions.emplace_back(std::string("bullshit"));
  world->setDefinitions(worldDefinitions);
  world->setDictionary(englishDictionary);

  /*
    std::vector<std::weak_ptr<word>> words;
    words.emplace_back(hello);
    words.emplace_back(world);
    englishDictionary->setWords(words);
  */
  std::cout << "[INFO] init objects out of transactions" << std::endl;

  // Make objects persistent and save their ids for later use.
  //
  {
    std::cout << "[INFO] open transaction" << std::endl;
    odb::core::transaction t(db->begin());

    english_id = db->persist(*englishDictionary);
    std::cout << "[INFO] persist english dictionary" << std::endl;

    hello_id = db->persist(*hello);
    std::cout << "[INFO] persist hello word" << std::endl;

    world_id = db->persist(*world);
    std::cout << "[INFO] persist world word" << std::endl;

    t.commit();

    std::cout << "[INFO] first commit" << std::endl;
  }

  BOOST_CHECK_EQUAL(2, printDataCount<word_stat>(db));
  BOOST_CHECK_EQUAL(1, printDataCount<dictionary_stat>(db));

  // Say hello to those have id under 2.
  //
  {
    odb::core::transaction t(db->begin());

    odb::result<word> r(db->query<word>(odb::query<word>::id < 10));

    for (odb::result<word>::iterator i(r.begin()); i != r.end(); ++i) {
      std::cout << "Word : " << i->getName() << " "
                << "!" << std::endl;
    }

    t.commit();
  }

  {
    odb::core::transaction t(db->begin());
    std::vector<std::weak_ptr<word>> words;
    std::shared_ptr<dictionary> english(db->load<dictionary>(english_id));
    words = english->getWords();
    BOOST_CHECK_EQUAL(2, words.size());
    t.commit();
  }

  /*
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
    std::shared_ptr<user> joe(db->load<user>(joe_id));
    std::shared_ptr<user> jane(db->load<user>(jane_id));
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
    std::shared_ptr<group> userGroup(db->load<group>(user_group_id));
    std::shared_ptr<user> joe(db->load<user>(joe_id));
    std::shared_ptr<user> jane(db->load<user>(jane_id));
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
  const std::string tok = UserDBEndpoint::getInstance()
                              ->connectUser(boost::property_tree::ptree(), frk)
                              .get<std::string>("data.token");

  std::cout << "New token found is \"" << tok << "\"" << std::endl;

  BOOST_CHECK_EQUAL(4, printUserCount(db));

  // John Doe is no longer in our database.
  boost::property_tree::ptree del;
  del.put("user.id", john_id);
  CRUDOrderDispatcher::deleteObject<user>(boost::property_tree::ptree(), del);

  */
}

BOOST_AUTO_TEST_SUITE_END()
