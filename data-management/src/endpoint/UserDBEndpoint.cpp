#include "./UserDBEndpoint.hpp"
#include <iostream>

std::unique_ptr<UserDBEndpoint> UserDBEndpoint::s_instance = nullptr;

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
std::unique_ptr<UserDBEndpoint> &UserDBEndpoint::getInstance() {
  if (nullptr == s_instance) {
    s_instance = std::make_unique<UserDBEndpoint>();
  }
  return s_instance;
}
