#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <memory>   // unique_ptr
#include <thread>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream
#include <string>  // std::string

#include <boost/property_tree/ptree.hpp>
#include <odb/transaction.hxx>

// create database access
#include "./connector/database.hxx"
#include "./endpoint/UserDBEndpoint.hpp"

#if defined(DATABASE_MYSQL)
#include "./model/mysql/user-odb.hxx"
#include "./model/mysql/user.hxx"
#elif defined(DATABASE_SQLITE)
#include "./model/sqlite/user-odb.hxx"
#include "./model/sqlite/user.hxx"
#elif defined(DATABASE_PGSQL)
#include "./model/pgsql/user-odb.hxx"
#include "./model/pgsql/user.hxx"
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include "./TCP/Server.hpp"

enum { max_length = 1024 };

/**
 * @brief Get the Data Base Access object
 *
 * @return std::unique_ptr<odb::core::database>
 */
std::unique_ptr<odb::core::database> getDataBaseAccess() {
  const char *const tempArgv[] = {"_", "--user", "odb_test", "--database",
                                  "data.db"};
  const uint8_t tempArgc = 5;

  return create_database(tempArgc, tempArgv);
}

void printUserCount(const std::unique_ptr<odb::core::database> &db) {
  odb::core::transaction t(db->begin());

  // The result of this (aggregate) query always has exactly one element
  // so use the query_value() shortcut.
  //
  const user_stat ps(db->query_value<user_stat>());

  std::cout << std::endl << "count  : " << ps.count << std::endl;

  t.commit();
}

void startTcpServer() {
  try {
    boost::asio::io_context ioContext;
    const hangman::tcp::Server server(ioContext, 50000);
    ioContext.run();
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

int32_t main(int argc, char * argv[]) {
  int32_t exitStatus = EXIT_SUCCESS;

  std::thread serv(startTcpServer);
  if (argc > 1 && std::string("lock").compare(argv[1])) {
    serv.join();
  } else {
    serv.detach();
  }

  try {
    std::unique_ptr<odb::core::database> db = create_database(argc, argv);

    printUserCount(db);

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
        std::cout << "Hello, " << i->getLogin() << " " << i->getPassword()
                  << "!" << std::endl;
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
    const std::string tok = UserDBEndpoint::connectUser(db, frk);

    std::cout << "New token found is \"" << tok << "\"" << std::endl;

    printUserCount(db);

    // John Doe is no longer in our database.
    UserDBEndpoint::deleteUser(db, john_id);

    printUserCount(db);

  } catch (const std::exception &ex) {
    std::cerr << "[ERROR] " << ex.what() << std::endl;
    exitStatus = EXIT_FAILURE;
  }
  return exitStatus;
}
