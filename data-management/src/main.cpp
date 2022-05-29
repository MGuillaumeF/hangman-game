#define DATABASE_SQLITE

#include <iostream>
#include <memory> // std::auto_ptr, unique_ptr

#include <odb/transaction.hxx>

#include "./connector/database.hxx" // create database access

#include "./model/user-odb.hxx"
#include "./model/user.hxx"

int32_t main(int argc, char *argv[]) {

  using namespace odb::core;

  char *exec_name = "./HangmanGameTest";
  char *user_key = "--user";
  char *user_value = "odb_test";
  char *database_key = "--database";
  char *database_value = "odb_test";
  char *tempArgv[] = {exec_name, user_key, user_value, database_key,
                      database_value};
  int tempArgc = 5;
  unique_ptr<database> db(create_database(tempArgc, tempArgv));

  unsigned long john_id;
  unsigned long joe_id;

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

  // Make objects persistent and save their ids for later use.
  //
  {
    transaction t(db->begin());

    john_id = db->persist(john);
    db->persist(jane);
    joe_id = db->persist(joe);

    t.commit();
  }

  typedef odb::query<user> query;
  typedef odb::result<user> result;

  // Say hello to those have id under 2.
  //
  {
    transaction t(db->begin());

    result r(db->query<user>(query::id < 3));

    for (result::iterator i(r.begin()); i != r.end(); ++i) {
      std::cout << "Hello, " << i->getLogin() << " " << i->getPassword() << "!"
                << std::endl;
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
  {
    transaction t (db->begin ());
    // Here we know that there can be only one Joe  in our
    // database so we use the query_one() shortcut instead of
    // manually iterating over the result returned by query().
    //
    std::unique_ptr<user> frank (
      db->query_one<user> (query::login == "Frank" &&
                             query::password == "password_4"));
    if (frank.get () != 0)
    {
      frank->setToken ("new token");
      db->update (* frank);
    }
    t.commit ();
  }

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
}
