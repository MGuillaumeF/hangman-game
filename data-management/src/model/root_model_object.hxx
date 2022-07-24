#ifndef __ROOT_MODEL_OBJECT_HXX__
#define __ROOT_MODEL_OBJECT_HXX__

#include <boost/property_tree/ptree.hpp>
#include <odb/core.hxx>
#include <string>
#include <ctime>

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
   * @brief Get the creation user of object
   *
   * @return const std::string& the creation user of object
   */
  const std::string &getCreatedBy() const { return m_created_by; };

  /**
   * @brief Get the creation timestamp of object
   *
   * @return const uint32_t& the creation timestamp of object
   */
  const uint32_t &getCreatedAt() const { return m_created_at; };

  /**
   * @brief Get the updater user of object
   *
   * @return const std::string& the updater user of object
   */
  const std::string &getUpdatedBy() const { return m_updated_by; };

  /**
   * @brief Get the update timestamp of object
   *
   * @return const uint32_t& the update timestamp of object
   */
  const uint32_t &getUpdatedAt() const { return m_updated_at; };

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
  template <typename T, typename std::enable_if<std::is_base_of<
                            root_model_object, T>::value>::type * = nullptr>
  static T parse(const boost::property_tree::ptree &property_tree) {
    T childObject;
    const boost::optional<uint32_t> id =
        property_tree.get_optional<uint32_t>("id");
    if (id) {
      childObject.setId(*id);
    }
    return childObject;
  };

protected:
  friend class odb::access;
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
