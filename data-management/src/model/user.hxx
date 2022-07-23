#ifndef __MODEL_USER_HXX__
#define __MODEL_USER_HXX__

#include <string>

#include <odb/core.hxx>

#include "./root_model_object.hxx"

/**
 * @brief class of Users in model
 *
 */
#pragma db object
class user final : public root_model_object {
public:
  /**
   * @brief Construct a new user object
   *
   */
  user() = default;

  /**
   * @brief Get the Login object
   *
   * @return const std::string&
   */
  const std::string &getLogin() const { return m_login; };

  /**
   * @brief Set the Login object
   *
   * @param login
   */
  void setLogin(const std::string &login) { m_login = login; };

  /**
   * @brief Get the Password object
   *
   * @return const std::string&
   */
  const std::string &getPassword() const { return m_password; };

  /**
   * @brief Set the Password object
   *
   * @param password
   */
  void setPassword(const std::string &password) { m_password = password; };

  /**
   * @brief Get the Salt User object
   *
   * @return const std::string&
   */
  const std::string &getSaltUser() const { return m_saltUser; };

  /**
   * @brief Set the Salt User object
   *
   * @param saltUser
   */
  void setSaltUser(const std::string &saltUser) { m_saltUser = saltUser; };

  /**
   * @brief Get the Salt Session object
   *
   * @return const std::string&
   */
  const std::string &getSaltSession() const { return m_saltSession; };

  /**
   * @brief Set the Salt Session object
   *
   * @param saltSession
   */
  void setSaltSession(const std::string &saltSession) {
    m_saltSession = saltSession;
  };

  /**
   * @brief Get the Token object
   *
   * @return const std::string&
   */
  const std::string &getToken() const { return m_token; };

  /**
   * @brief Set the Token object
   *
   * @param token
   */
  void setToken(const std::string &token) { m_token = token; };
  /**
   * @brief Get the Last connection object
   *
   * @return const uint32_t&
   */
  const uint32_t &getLastConnection() const { return m_lastConnection; };

  /**
   * @brief Set the last connection object
   *
   * @param lastConnection
   */
  void setLastConnection(const uint32_t &lastConnection) {
    m_lastConnection = lastConnection;
  };

  /**
   * @brief method to check if all fields of user are valid
   *
   * @return true The content of user object is valid
   * @return false The content of user object is invalid
   */
  bool isValid() { return getLogin().size() > 3; }

  /**
   * @brief method to convert object to property tree
   *
   * @return The object on property tree format
   */
  boost::property_tree::ptree toPtree() {
    return boost::property_tree::ptree();
  }

  /**
   * @brief method to extract object from property tree
   *
   * @return The object found
   */
  static user parse(boost::property_tree::ptree property_tree) {
    user parsedUser();
    return parsedUser;
  }

private:
#pragma db unique not_null type("VARCHAR(255)")
  std::string m_login;
#pragma db options() options("CHECK(password != '')")
  std::string m_password;
#pragma db options() options("CHECK(saltUser != '')")
  std::string m_saltUser;
  std::string m_saltSession;
  std::string m_token;
  uint32_t m_lastConnection;
};

#pragma db object(user)

#pragma db view object(user)
struct user_stat {
#pragma db column("count(" + user::m_id + ")")
  std::size_t count;
};

#endif // USER_HXX
