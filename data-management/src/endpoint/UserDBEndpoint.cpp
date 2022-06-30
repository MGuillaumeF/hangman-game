#include "./UserDBEndpoint.hpp"
#include "./DataAccess.hpp"
#include <iostream>

std::unique_ptr<UserDBEndpoint> UserDBEndpoint::s_instance = nullptr;

/**
 * @brief Create User(s) object(s)
 *
 * @param data The property tree data of user(s) to create
 * @return boost::property_tree::ptree The user(s) id(s) list after create
 */
boost::property_tree::ptree
UserDBEndpoint::createUser(const boost::property_tree::ptree &data) const {
  std::list<user> users;
  boost::property_tree::ptree ids;

  const boost::optional<const boost::property_tree::ptree &> userItem =
      data.get_child_optional("user");
  if (userItem) {
    users.push_back(UserDBEndpoint::parse(*userItem));

  } else {
    const boost::optional<const boost::property_tree::ptree &> userList =
        data.get_child_optional("users");
    if (userList) {
      for (const auto &userItem2 : (*userList)) {
        if ("user" == userItem2.first) {
          users.push_back(UserDBEndpoint::parse(userItem2.second));
        }
      }
    }
  }
  odb::core::transaction t(m_db->begin());
  for (user userToPersist : users) {
    const uint32_t id = m_db->persist(userToPersist);

    boost::property_tree::ptree userId;
    userId.put("id", id);
    ids.add_child("user", userId);
  }
  t.commit();
  return ids;
}

/**
 * @brief function to delete user by id
 *
 * @param data The property tree data of user(s) to delete
 */
void UserDBEndpoint::deleteUser(const boost::property_tree::ptree &data) const {

  std::list<uint32_t> userIds;
  const boost::optional<const boost::property_tree::ptree &> userItem =
      data.get_child_optional("user");
  if (userItem) {
    userIds.push_back((*userItem).get<uint32_t>("id"));

  } else {
    const boost::optional<const boost::property_tree::ptree &> userList =
        data.get_child_optional("users");
    if (userList) {
      for (const auto &userItem2 : (*userList)) {
        if ("user" == userItem2.first) {
          userIds.push_back(userItem2.second.get<uint32_t>("id"));
        }
      }
    }
  }
  odb::core::transaction t(m_db->begin());
  for (const uint32_t userToDelete : userIds) {
    m_db->erase<user>(userToDelete);
  }
  t.commit();
}

/**
 * @brief function to connect user by login and password
 *
 * @param data The property tree data of user to connect
 * @return std::string The new token of connected user
 */
std::string
UserDBEndpoint::connectUser(const boost::property_tree::ptree &data) const {
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
 * @brief methode to get unique instance of user endpoint
 *
 * @return UserDBEndpoint* user endpoint pointer of single instance
 */
std::unique_ptr<UserDBEndpoint>& UserDBEndpoint::getInstance() {
  if (nullptr == s_instance) {
    s_instance = std::unique_ptr<UserDBEndpoint>(new UserDBEndpoint());
  }
  return s_instance;
}
/**
 * @brief Construct a new User DB Endpoint:: User DB Endpoint object
 *
 */
UserDBEndpoint::UserDBEndpoint(){
  m_db = DataAccess::getDatabaseAccess();
}

/**
 * @brief method to parse user from property tree
 *
 * @param data The boost proerty tree of user
 * @return user User object
 */
user UserDBEndpoint::parse(const boost::property_tree::ptree &data) {
  user newUser;
  newUser.setLogin(data.get<std::string>("login"));
  newUser.setPassword(data.get<std::string>("password"));
  newUser.setSaltUser(data.get<std::string>("salt_user"));
  return newUser;
}
