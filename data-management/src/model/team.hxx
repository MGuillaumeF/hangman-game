
/**
 * @brief team.hxx DO NOT MODIFY THIS FILE, this file is a generated model class
 */

#ifndef __GENERATED_MODEL_OBJECT_TEAM_HXX__
#define __GENERATED_MODEL_OBJECT_TEAM_HXX__

#include "./chat.hxx"
#include "./root_model_object.hxx"

#include <memory>
#include <string>
#include <vector>

class user;
class chat;

/**
 * @brief class of team object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class team final : public root_model_object {
private:
  friend class odb::access;
  std::string m_name;
#pragma db value_not_null inverse(m_teams)
  std::vector<std::weak_ptr<user>> m_members;
#pragma db not_null
  std::shared_ptr<chat> m_chat;

public:
  /**
   * @brief Construct a new team object
   *
   */
  team() = default;

  /**
   * @brief Set the name of object
   *
   * @param name The name of object
   */
  void setName(const std::string &name) { m_name = name; };

  /**
   * @brief Get the name of object
   *
   * @return const std::string& the name of object
   */
  const std::string &getName() const { return m_name; };

  /**
   * @brief Set the members of object
   *
   * @param members The members of object
   */
  void setMembers(const std::vector<std::weak_ptr<user>> &members) {
    m_members = members;
  };

  /**
   * @brief Get the members of object
   *
   * @return const std::vector<std::weak_ptr<user>>& the members of object
   */
  const std::vector<std::weak_ptr<user>> &getMembers() const {
    return m_members;
  };

  /**
   * @brief Set the chat of object
   *
   * @param chat The chat of object
   */
  void setChat(const std::shared_ptr<chat> &chat) { m_chat = chat; };

  /**
   * @brief Get the chat of object
   *
   * @return const std::shared_ptr<chat>& the chat of object
   */
  const std::shared_ptr<chat> &getChat() const { return m_chat; };

  /**
   * @brief method to extract object from property tree
   *
   * @return The team found
   */
  static std::unique_ptr<team>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<team> parsedObject =
        root_model_object::parse<team>(property_tree);
    const boost::optional<std::string> name =
        property_tree.get_optional<std::string>("name");
    if (name) {
      parsedObject->setName(*name);
    }

    return parsedObject;
  }

  /**
   * @brief method to check if all fields of object are valid
   *
   * @return the error vector of validation
   */
  std::vector<model_error> getErrors() const {
    std::vector<model_error> errors;
    // TODO add implementation
    return errors;
  }

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "team"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "teams"; }
};

#pragma db object(team)

#pragma db view object(team)
struct team_stat {
#pragma db column("count(" + team::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_TEAM_HXX__

#ifdef ODB_COMPILER
#include "./user.hxx"
#endif
