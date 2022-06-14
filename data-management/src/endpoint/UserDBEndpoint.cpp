#include "./UserDBEndpoint.hpp"
#include <iostream>

UserDBEndpoint *UserDBEndpoint::s_instance = nullptr;

/**
 * @brief Create a User object
 *
 * @param data The property tree data of user to create
 * @return uint32_t The user id after create
 */
uint32_t UserDBEndpoint::createUser(const boost::property_tree::ptree &data) {
  std::cout << "here";
  user newUser;

  newUser.setLogin(data.get<std::string>("login"));
  newUser.setPassword(data.get<std::string>("password"));
  newUser.setSaltUser(data.get<std::string>("salt_user"));

  odb::core::transaction t(m_db->begin());
  const uint32_t id = m_db->persist(newUser);
  t.commit();

  return id;
}

/**
 * @brief function to delete user by id
 *
 * @param m_db The database access
 * @param id The id of user t delete
 */
void UserDBEndpoint::deleteUser(const uint32_t &id) {

  odb::core::transaction t(m_db->begin());
  m_db->erase<user>(id);
  t.commit();
}

/**
 * @brief function to connect user by login and password
 *
 * @param m_db The database access
 * @param data The property tree data of user to connect
 * @return std::string The new token of connected user
 */
std::string
UserDBEndpoint::connectUser(const boost::property_tree::ptree &data) {
  std::string token = "";
  odb::core::transaction t(m_db->begin());
  std::cout << "here1 " << data.get<std::string>("login") << ":"
            << data.get<std::string>("password") << std::endl;

  const std::unique_ptr<user> foundUser(m_db->query_one<user>(
      odb::query<user>::login == data.get<std::string>("login") &&
      odb::query<user>::password == data.get<std::string>("password")));
  if (foundUser.get() != nullptr) {
    token = "new token";
    foundUser->setToken(token);
    m_db->update(*foundUser);
  }

  t.commit();

  return token;
}

UserDBEndpoint *UserDBEndpoint::getInstance() {
  if (s_instance == nullptr) {
    s_instance = new UserDBEndpoint();
  }
  return s_instance;
}

UserDBEndpoint::UserDBEndpoint() {
  char *exec_name = "./HangmanGameTest";
  char *user_key = "--user";
  char *user_value = "odb_test";
  char *database_key = "--database";
  char *database_value = "odb_test";
  char *tempArgv[] = {exec_name, user_key, user_value, database_key,
                      database_value};
  int tempArgc = 5;

  m_db = create_database(tempArgc, tempArgv);
}