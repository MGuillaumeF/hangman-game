#ifndef __MODEL_GROUP_HXX__
#define __MODEL_GROUP_HXX__

#include "./root_model_object.hxx"
#include <memory>
#include <string>
#include <vector>

#include <odb/core.hxx>

/**
 * @brief class of Groups in model
 *
 */
#pragma db object
class group final : public root_model_object {
public:
  /**
   * @brief Construct a new group object
   *
   */
  group() = default;

  /**
   * @brief Get the name of group object
   *
   * @return const std::string&
   */
  const std::string &getName() const { return m_name; };

  /**
   * @brief Set the name of group object
   *
   * @param name The name of group
   */
  void setName(const std::string &name) { m_name = name; };

  /**
   * @brief method to check if all fields of group are valid
   *
   * @return true The content of group object is valid
   * @return false The content of group object is invalid
   */
  static bool isValid(const group &grp) { return grp.getName().size() > 3; }

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
  static group parse(const boost::property_tree::ptree &property_tree) {
    group parsedGroup;
    return parsedGroup;
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

private:
  friend class odb::access;

#pragma db options() options("CHECK(name != '')")
  std::string m_name;
};
#pragma db object(group)

#pragma db view object(group)
struct group_stat {
#pragma db column("count(" + group::m_id + ")")
  std::size_t count;
};

#endif // __MODEL_GROUP_HXX__
