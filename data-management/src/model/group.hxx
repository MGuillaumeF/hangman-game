
/**
 * @brief group.hxx DO NOT MODIFY THIS FILE, this file is a generated model
 * class
 */

#ifndef __GENERATED_MODEL_OBJECT_GROUP_HXX__
#define __GENERATED_MODEL_OBJECT_GROUP_HXX__

#include "./root_model_object.hxx"

#include <memory>
#include <string>
#include <vector>

class user;

/**
 * @brief class of group object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class group final : public root_model_object {
private:
  friend class odb::access;
  std::string m_name;
#pragma db value_not_null inverse(m_groups)
  std::vector<std::weak_ptr<user>> m_members;

public:
  /**
   * @brief Construct a new group object
   *
   */
  group() = default;

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
   * @brief method to extract object from property tree
   *
   * @return The group found
   */
  static std::unique_ptr<group>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<group> parsedObject =
        root_model_object::parse<group>(property_tree);
    const boost::optional<std::string> name =
        property_tree.get_optional<std::string>("name");
    if (name) {
      parsedObject->setName(*name);
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
  static std::string getObjectType() { return "group"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "groups"; }
};

#pragma db object(group)

#pragma db view object(group)
struct group_stat {
#pragma db column("count(" + group::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_GROUP_HXX__

#ifdef ODB_COMPILER
#include "./user.hxx"
#endif
