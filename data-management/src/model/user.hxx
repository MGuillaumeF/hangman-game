
/**
 * @brief user.hxx DO NOT MODIFY THIS FILE, this file is a generated model class
 */

#ifndef __GENERATED_MODEL_OBJECT_USER_HXX__
#define __GENERATED_MODEL_OBJECT_USER_HXX__

#include "./chat.hxx"
#include "./group.hxx"
#include "./party.hxx"
#include "./root_model_object.hxx"
#include "./team.hxx"

#include <ctime>
#include <memory>
#include <string>
#include <vector>

class group;
class team;
class chat;
class party;

/**
 * @brief class of user object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class user final : public root_model_object {
private:
  friend class odb::access;
  std::string m_login;
  std::string m_password;
  std::string m_salt_user;
  std::string m_salt_session;
  std::string m_token;
  std::time_t m_last_connection;
#pragma db value_not_null unordered
  std::vector<std::shared_ptr<user>> m_friends;
#pragma db value_not_null unordered
  std::vector<std::shared_ptr<group>> m_groups;
#pragma db value_not_null unordered
  std::vector<std::shared_ptr<team>> m_teams;
#pragma db value_not_null unordered
  std::vector<std::shared_ptr<chat>> m_chats;
#pragma db value_not_null unordered
  std::vector<std::shared_ptr<party>> m_parties;

public:
  /**
   * @brief Construct a new user object
   *
   */
  user() = default;

  /**
   * @brief Set the login of object
   *
   * @param login The login of object
   */
  void setLogin(const std::string &login) { m_login = login; };

  /**
   * @brief Get the login of object
   *
   * @return const std::string& the login of object
   */
  const std::string &getLogin() const { return m_login; };

  /**
   * @brief Set the password of object
   *
   * @param password The password of object
   */
  void setPassword(const std::string &password) { m_password = password; };

  /**
   * @brief Get the password of object
   *
   * @return const std::string& the password of object
   */
  const std::string &getPassword() const { return m_password; };

  /**
   * @brief Set the salt_user of object
   *
   * @param salt_user The salt_user of object
   */
  void setSaltUser(const std::string &salt_user) { m_salt_user = salt_user; };

  /**
   * @brief Get the salt_user of object
   *
   * @return const std::string& the salt_user of object
   */
  const std::string &getSaltUser() const { return m_salt_user; };

  /**
   * @brief Set the salt_session of object
   *
   * @param salt_session The salt_session of object
   */
  void setSaltSession(const std::string &salt_session) {
    m_salt_session = salt_session;
  };

  /**
   * @brief Get the salt_session of object
   *
   * @return const std::string& the salt_session of object
   */
  const std::string &getSaltSession() const { return m_salt_session; };

  /**
   * @brief Set the token of object
   *
   * @param token The token of object
   */
  void setToken(const std::string &token) { m_token = token; };

  /**
   * @brief Get the token of object
   *
   * @return const std::string& the token of object
   */
  const std::string &getToken() const { return m_token; };

  /**
   * @brief Set the last_connection of object
   *
   * @param last_connection The last_connection of object
   */
  void setLastConnection(const std::time_t &last_connection) {
    m_last_connection = last_connection;
  };

  /**
   * @brief Get the last_connection of object
   *
   * @return const std::time_t& the last_connection of object
   */
  const std::time_t &getLastConnection() const { return m_last_connection; };

  /**
   * @brief Set the friends of object
   *
   * @param friends The friends of object
   */
  void setFriends(const std::vector<std::shared_ptr<user>> &friends) {
    m_friends = friends;
  };

  /**
   * @brief Get the friends of object
   *
   * @return const std::vector<std::shared_ptr<user>>& the friends of object
   */
  const std::vector<std::shared_ptr<user>> &getFriends() const {
    return m_friends;
  };

  /**
   * @brief Set the groups of object
   *
   * @param groups The groups of object
   */
  void setGroups(const std::vector<std::shared_ptr<group>> &groups) {
    m_groups = groups;
  };

  /**
   * @brief Get the groups of object
   *
   * @return const std::vector<std::shared_ptr<group>>& the groups of object
   */
  const std::vector<std::shared_ptr<group>> &getGroups() const {
    return m_groups;
  };

  /**
   * @brief Set the teams of object
   *
   * @param teams The teams of object
   */
  void setTeams(const std::vector<std::shared_ptr<team>> &teams) {
    m_teams = teams;
  };

  /**
   * @brief Get the teams of object
   *
   * @return const std::vector<std::shared_ptr<team>>& the teams of object
   */
  const std::vector<std::shared_ptr<team>> &getTeams() const {
    return m_teams;
  };

  /**
   * @brief Set the chats of object
   *
   * @param chats The chats of object
   */
  void setChats(const std::vector<std::shared_ptr<chat>> &chats) {
    m_chats = chats;
  };

  /**
   * @brief Get the chats of object
   *
   * @return const std::vector<std::shared_ptr<chat>>& the chats of object
   */
  const std::vector<std::shared_ptr<chat>> &getChats() const {
    return m_chats;
  };

  /**
   * @brief Set the parties of object
   *
   * @param parties The parties of object
   */
  void setParties(const std::vector<std::shared_ptr<party>> &parties) {
    m_parties = parties;
  };

  /**
   * @brief Get the parties of object
   *
   * @return const std::vector<std::shared_ptr<party>>& the parties of object
   */
  const std::vector<std::shared_ptr<party>> &getParties() const {
    return m_parties;
  };

  /**
   * @brief method to extract object from property tree
   *
   * @return The user found
   */
  static std::unique_ptr<user>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<user> parsedObject =
        root_model_object::parse<user>(property_tree);
    const boost::optional<std::string> login =
        property_tree.get_optional<std::string>("login");
    if (login) {
      parsedObject->setLogin(*login);
    }
    const boost::optional<std::string> password =
        property_tree.get_optional<std::string>("password");
    if (password) {
      parsedObject->setPassword(*password);
    }
    const boost::optional<std::string> salt_user =
        property_tree.get_optional<std::string>("salt_user");
    if (salt_user) {
      parsedObject->setSaltUser(*salt_user);
    }
    const boost::optional<std::string> salt_session =
        property_tree.get_optional<std::string>("salt_session");
    if (salt_session) {
      parsedObject->setSaltSession(*salt_session);
    }
    const boost::optional<std::string> token =
        property_tree.get_optional<std::string>("token");
    if (token) {
      parsedObject->setToken(*token);
    }
    const boost::optional<std::time_t> last_connection =
        property_tree.get_optional<std::time_t>("last_connection");
    if (last_connection) {
      parsedObject->setLastConnection(*last_connection);
    }

    return parsedObject;
  }

  /**
   * @brief method to get object's errors
   *
   * @return The error vector
   */
  std::vector<model_error> getErrors() {
    std::vector<model_error> errors;
    // TODO add implementation
    return errors;
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
};

#pragma db object(user)

#pragma db view object(user)
struct user_stat {
#pragma db column("count(" + user::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_USER_HXX__

#ifdef ODB_COMPILER

#endif
