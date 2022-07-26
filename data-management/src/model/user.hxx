#ifndef __MODEL_USER_HXX__
#define __MODEL_USER_HXX__

#include <memory>
#include <string>
#include <vector>

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
  static user parse(const boost::property_tree::ptree &property_tree) {
    user parsedUser = root_model_object::parse<user>(property_tree);
    parsedUser.setLogin(property_tree.get<std::string>("login"));
    parsedUser.setPassword(property_tree.get<std::string>("password"));
    parsedUser.setSaltUser(property_tree.get<std::string>("salt_user"));
    const boost::optional<std::string> saltSession =
        property_tree.get_optional<std::string>("salt_session");
    if (saltSession) {
      parsedUser.setSaltSession(*saltSession);
    }
    const boost::optional<std::string> token =
        property_tree.get_optional<std::string>("token");
    if (token) {
      parsedUser.setToken(*token);
    }
    const boost::optional<uint32_t> lastConnection =
        property_tree.get_optional<uint32_t>("last_connection");
    if (lastConnection) {
      parsedUser.setLastConnection(*lastConnection);
    }
    return parsedUser;
  }

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "user"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "users"; }

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
#pragma db value_not_null unordered
  std::vector<std::shared_ptr<user>> m_friends;
};

#pragma db object(user)

#pragma db view object(user)
struct user_stat {
#pragma db column("count(" + user::m_id + ")")
  std::size_t count;
};

#endif // USER_HXX
