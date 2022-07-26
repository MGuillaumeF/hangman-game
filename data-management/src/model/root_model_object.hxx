#ifndef __ROOT_MODEL_OBJECT_HXX__
#define __ROOT_MODEL_OBJECT_HXX__

#include <boost/property_tree/ptree.hpp>
#include <ctime>
#include <odb/core.hxx>
#include <string>

/**
 * @brief class of Root Model Object in model
 *
 */
#pragma db object
class root_model_object {

public:
  /**
   * @brief Construct a new Root Model Object
   *
   */
  root_model_object() = default;

  /**
   * @brief Get the Id object
   *
   * @return const uint32_t&
   */
  const uint32_t &getId() const { return m_id; };

  /**
   * @brief Set the Id object
   *
   * @param id
   */
  void setId(const uint32_t &id) { m_id = id; };

  /**
   * @brief Get the version of object
   *
   * @return const uint32_t& the version of object
   */
  const uint32_t &getVersion() const { return m_version; };

  /**
   * @brief Set the Version of object
   *
   * @param version The version of object
   */
  void setVersion(const uint32_t &version) { m_version = version; };

  /**
   * @brief Get the creation user of object
   *
   * @return const std::string& the creation user of object
   */
  const std::string &getCreatedBy() const { return m_created_by; };

  /**
   * @brief Set the creation user of object
   *
   * @param created_by The creation user of object
   */
  void setCreatedBy(const std::string &created_by) {
    m_created_by = created_by;
  };

  /**
   * @brief Get the creation datetime of object
   *
   * @return const std::time_t& the creation datetime of object
   */
  const std::time_t &getCreatedAt() const { return m_created_at; };

  /**
   * @brief Set the create datetime of object
   *
   * @param created_at The create datetime of object
   */
  void setCreatedAt(const std::time_t &created_at) {
    m_created_at = created_at;
  };

  /**
   * @brief Get the updater user of object
   *
   * @return const std::string& the updater user of object
   */
  const std::string &getUpdatedBy() const { return m_updated_by; };

  /**
   * @brief Set the update user of object
   *
   * @param updated_by The update user of object
   */
  void setUpdatedBy(const std::string &updated_by) {
    m_updated_by = updated_by;
  };

  /**
   * @brief Get the update datetime of object
   *
   * @return const std::time_t& the update datetime of object
   */
  const std::time_t &getUpdatedAt() const { return m_updated_at; };

  /**
   * @brief Set the update datetime of object
   *
   * @param updated_at The update datetime of object
   */
  void setUpdatedAt(const std::time_t &updated_at) {
    m_updated_at = updated_at;
  };

  /**
   * @brief method to check if all fields of object are valid
   *
   * @return true The content of object is valid
   * @return false The content of object is invalid
   */
  bool isValid();

  /**
   * @brief method to convert object to property tree
   *
   * @return The object on property tree format
   */
  boost::property_tree::ptree toPtree();

  /**
   * @brief method to extract object from property tree
   *
   * @return The object found
   */
  template <typename T, typename std::enable_if_t<std::is_base_of<
                            root_model_object, T>::value>* = nullptr>
  static T parse(const boost::property_tree::ptree &property_tree) {
    T childObject;
    const boost::optional<uint32_t> id =
        property_tree.get_optional<uint32_t>("id");
    if (id) {
      childObject.setId(*id);
    }
    const boost::optional<uint32_t> version =
        property_tree.get_optional<uint32_t>("version");
    if (version) {
      childObject.setVersion(*version);
    }
    const boost::optional<std::string> created_by =
        property_tree.get_optional<std::string>("created_by");
    if (created_by) {
      childObject.setCreatedBy(*created_by);
    }
    const boost::optional<std::string> updated_by =
        property_tree.get_optional<std::string>("updated_by");
    if (updated_by) {
      childObject.setUpdatedBy(*updated_by);
    }
    return childObject;
  };

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "root_model_object"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "root_model_objects"; }

protected:
  friend class odb::access;

private:
#pragma db id auto
  uint32_t m_id = 0;
  uint32_t m_version = 0;
  std::string m_created_by = "anonymous";
  std::time_t m_created_at = std::time(nullptr);
  std::string m_updated_by = "anonymous";
  std::time_t m_updated_at = std::time(nullptr);
};

#pragma db object(root_model_object)

#endif // ROOT_MODEL_OBJECT_HXX
