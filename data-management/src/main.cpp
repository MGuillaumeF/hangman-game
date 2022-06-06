#define DATABASE_SQLITE

#include <exception>
#include <iostream>
#include <memory> // unique_ptr
#include <thread>

#include <boost/property_tree/ptree.hpp>
#include <odb/transaction.hxx>

// create database access
#include "./connector/database.hxx"

#include "./model/user-odb.hxx"
#include "./model/user.hxx"

#include "./TCP/Server.hpp"

enum { max_length = 1024 };

/**
 * @brief Get the Data Base Access object
 *
 * @return std::unique_ptr<odb::core::database>
 */
std::unique_ptr<odb::core::database> getDataBaseAccess() {
  char *tempArgv[] = {"_", "--user", "odb_test", "--database", "data.db"};
  int32_t tempArgc = static_cast<int32_t>(sizeof(tempArgv));

  return create_database(tempArgc, tempArgv);
}
/**
 * @brief Create a User object
 *
 * @param data The property tree data of user to create
 * @return uint32_t The user id after create
 */
uint32_t createUser(const std::unique_ptr<odb::core::database> &db,
                    const boost::property_tree::ptree &data) {

  user newUser;

  newUser.setLogin(data.get<std::string>("login"));
  newUser.setPassword(data.get<std::string>("password"));
  newUser.setSaltUser(data.get<std::string>("salt_user"));

  odb::core::transaction t(db->begin());
  const uint32_t id = db->persist(newUser);
  t.commit();

  return id;
}

/**
 * @brief function to delete user by id
 *
 * @param db The database access
 * @param id The id of user t delete
 */
void deleteUser(const std::unique_ptr<odb::core::database> &db,
                const uint32_t &id) {
  odb::core::transaction t(db->begin());
  db->erase<user>(id);
  t.commit();
}

/**
 * @brief function to connect user by login and password
 *
 * @param db The database access
 * @param data The property tree data of user to connect
 * @return std::string The new token of connected user
 */
std::string connectUser(const std::unique_ptr<odb::core::database> &db,
                        const boost::property_tree::ptree &data) {
  std::string token = "";
  odb::core::transaction t(db->begin());
  std::cout << "here1 " << data.get<std::string>("login") << ":"
            << data.get<std::string>("password") << std::endl;

  const std::unique_ptr<user> foundUser(db->query_one<user>(
      odb::query<user>::login == data.get<std::string>("login") &&
      odb::query<user>::password == data.get<std::string>("password")));
  if (foundUser.get() != nullptr) {
    token = "new token";
    foundUser->setToken(token);
    db->update(*foundUser);
  }

  t.commit();

  return token;
}

void printUserCount(const std::unique_ptr<odb::core::database> &db) {
  odb::core::transaction t(db->begin());

  // The result of this (aggregate) query always has exactly one element
  // so use the query_value() shortcut.
  //
  user_stat ps(db->query_value<user_stat>());

  std::cout << std::endl << "count  : " << ps.count << std::endl;

  t.commit();
}

void startTcpServer() {
  try {
    boost::asio::io_context ioContext;
    hangman::tcp::Server server(ioContext, 50000);
    ioContext.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

int32_t main(int argc, char *argv[]) {
  int32_t exitStatus = EXIT_SUCCESS;

  std::thread serv(startTcpServer);
  serv.detach();

  try {
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::socket s(io_context);
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("localhost", "50000"));

    std::cout << "Enter message: ";
    // char request[max_length];
    // std::cin.getline(request, max_length);
    char* request = "this is a request test";
    size_t request_length = std::strlen(request);
    boost::asio::write(s, boost::asio::buffer(request, request_length));

    char reply[max_length];
    size_t reply_length =
        boost::asio::read(s, boost::asio::buffer(reply, request_length));
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";

    s.close();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
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
    const std::string tok = connectUser(db, frk);

    std::cout << "New token found is \"" << tok << "\"" << std::endl;

    printUserCount(db);

    // John Doe is no longer in our database.
    deleteUser(db, john_id);

    printUserCount(db);

  } catch (const std::exception &ex) {
    std::cerr << "[ERROR] " << ex.what() << std::endl;
    exitStatus = EXIT_FAILURE;
  }
  return exitStatus;
}
