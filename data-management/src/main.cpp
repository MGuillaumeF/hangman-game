#define DATABASE_SQLITE
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>
#include <memory> // unique_ptr
#include <odb/transaction.hxx>
// create database access
#include "./connector/database.hxx"

#include "./model/user-odb.hxx"
#include "./model/user.hxx"

std::unique_ptr<odb::core::database> getDataBaseAccess() {
  char *exec_name = "./HangmanGameTest";
  char *user_key = "--user";
  char *user_value = "odb_test";
  char *database_key = "--database";
  char *database_value = "odb_test";
  char *tempArgv[] = {exec_name, user_key, user_value, database_key,
                      database_value};
  int tempArgc = 5;

  return create_database(tempArgc, tempArgv);
}
/**
 * @brief Create a User object
 *
 * @param data
 * @return uint32_t
 */
uint32_t createUser(const odb::core::database &db,
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
 * @brief
 *
 * @param data
 * @return std::string
 */
std::string connectUser(const odb::core::database &db,
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

int32_t main(int argc, char *argv[]) {
  int32_t result = EXIT_SUCCESS;

  try {
    std::unique_ptr<odb::core::database> db = getDataBaseAccess();

    using namespace odb::core;

    uint32_t john_id;
    uint32_t joe_id;

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
      transaction t(db->begin());

      john_id = db->persist(john);
      db->persist(jane);
      joe_id = db->persist(joe);
      db->persist(frank);

      t.commit();
    }

    typedef odb::query<user> query;
    typedef odb::result<user> result;

    // Say hello to those have id under 2.
    //
    {
      transaction t(db->begin());

      result r(db->query<user>(query::id < 10));

      for (result::iterator i(r.begin()); i != r.end(); ++i) {
        std::cout << "Hello, " << i->getLogin() << " " << i->getPassword()
                  << "!" << std::endl;
      }

      t.commit();
    }

    // Joe is logged, so update his token.
    //
    {
      transaction t(db->begin());

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

    std::cout << "New token found is \"" << tok << std::endl;

    // Print some statistics about all the people in our database.
    //
    {
      transaction t(db->begin());

      // The result of this (aggregate) query always has exactly one element
      // so use the query_value() shortcut.
      //
      user_stat ps(db->query_value<user_stat>());

      std::cout << std::endl << "count  : " << ps.count << std::endl;

      t.commit();
    }

    // John Doe is no longer in our database.
    //
    {
      transaction t(db->begin());
      db->erase<user>(john_id);
      t.commit();
    }

  } catch (const std::exception &ex) {
    std::cerr << "[ERROR] " << ex.what() << std::endl;
    result = EXIT_FAILURE;
  }
  return result;
}
