#include "./UserDBEndpoint.hpp"
#include "./StatusCode.hpp"
#include <iostream>

/**
 * @brief database access pointer of single instance
 *
 */
std::unique_ptr<UserDBEndpoint> UserDBEndpoint::s_instance = nullptr;

/**
 * @brief function to connect user by login and password
 *
 * @param properties The properties part of request
 * @param data The property tree data of user to connect
 * @return The result of request process
 */
boost::property_tree::ptree
UserDBEndpoint::connectUser(const boost::property_tree::ptree &properties,
                            const boost::property_tree::ptree &data) {
  boost::property_tree::ptree response;
  std::string token = "";
  odb::core::transaction t(m_db->begin());
  std::cout << "user connection " << data.get<std::string>("login") << ":"
            << data.get<std::string>("password") << std::endl;

  std::shared_ptr<user> foundUser(m_db->query_one<user>(
      odb::query<user>::login == data.get<std::string>("login") &&
      odb::query<user>::password == data.get<std::string>("password")));
  if (nullptr != foundUser.get()) {
    m_connectedUsers.emplace(foundUser->getLogin());
    token = "new token";
    foundUser->setLastConnection(std::time(nullptr));
    foundUser->setToken(token);
    m_db->update(*foundUser);
    response.put("status-code", uint16_t(status_code::OK));
    response.put("data.token", token);
  } else {
    response.put("status-code", uint16_t(status_code::NOT_FOUND));
  }

  t.commit();

  return response;
}

/**
 * @brief function to disconnect user by login and token
 *
 * @param properties The properties part of request
 * @param data The property tree data of user to disconnect
 * @return The result of request process
 */
boost::property_tree::ptree
UserDBEndpoint::disconnectUser(const boost::property_tree::ptree &properties,
                               const boost::property_tree::ptree &data) {
  boost::property_tree::ptree response;
  odb::core::transaction t(m_db->begin());
  std::cout << "user disconnection " << std::endl;
  std::shared_ptr<user> foundUser(m_db->query_one<user>(
      odb::query<user>::login == data.get<std::string>("login") &&
      odb::query<user>::token == data.get<std::string>("token")));
  if (nullptr != foundUser.get()) {
    m_connectedUsers.erase(m_connectedUsers.find(foundUser->getLogin()));
    const std::string token = "";
    foundUser->setToken(token);
    m_db->update(*foundUser);
    response.put("status-code", uint16_t(status_code::OK));
  } else {
    response.put("status-code", uint16_t(status_code::NOT_FOUND));
  }

  t.commit();

  return response;
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
