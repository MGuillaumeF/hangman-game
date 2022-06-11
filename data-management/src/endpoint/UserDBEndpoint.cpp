#include "./UserDBEndpoint.hpp"
#include <iostream>
/**
 * @brief Create a User object
 *
 * @param data The property tree data of user to create
 * @return uint32_t The user id after create
 */
uint32_t
UserDBEndpoint::createUser(const std::unique_ptr<odb::core::database> &db,
                           const boost::property_tree::ptree &data) {
  std::cout << "here";
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
void UserDBEndpoint::deleteUser(const std::unique_ptr<odb::core::database> &db,
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
std::string
UserDBEndpoint::connectUser(const std::unique_ptr<odb::core::database> &db,
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