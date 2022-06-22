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
 * @param data The property tree data of user to connect
 * @return std::string The new token of connected user
 */
std::string
UserDBEndpoint::connectUser(const boost::property_tree::ptree &data) {
  std::string token = "";
  odb::core::transaction t(m_db->begin());
  std::cout << "user connection " << data.get<std::string>("login") << ":"
            << data.get<std::string>("password") << std::endl;

  const std::unique_ptr<user> foundUser(m_db->query_one<user>(
      odb::query<user>::login == data.get<std::string>("login") &&
      odb::query<user>::password == data.get<std::string>("password")));
  if (nullptr != foundUser.get()) {
    token = "new token";
    foundUser->setToken(token);
    m_db->update(*foundUser);
  }

  t.commit();

  return token;
}
/**
 * @brief methode to get unique instance of USer endpointe
 *
 * @param db The database access pointer
 * @return UserDBEndpoint* user endpoint pointer of single instance
 */
UserDBEndpoint *UserDBEndpoint::getInstance(const odb::core::database * const db) {
  if (nullptr == s_instance && nullptr != db) {
    s_instance = new UserDBEndpoint(db);
  }
  return s_instance;
}
/**
 * @brief Construct a new User D B Endpoint:: User D B Endpoint object
 *
 * @param db The database access pointer
 */
UserDBEndpoint::UserDBEndpoint(const odb::core::database * const db) { m_db = db; }
