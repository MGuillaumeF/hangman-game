
/**
 * @brief root_model_object.hxx DO NOT MODIFY THIS FILE, this file is a
 * generated model class
 */

#ifndef __GENERATED_MODEL_OBJECT_ROOT_MODEL_OBJECT_HXX__
#define __GENERATED_MODEL_OBJECT_ROOT_MODEL_OBJECT_HXX__

#include "./model_error.hpp"
#include <boost/property_tree/ptree.hpp>
#include <ctime>
#include <list>
#include <odb/core.hxx>
#include <string>

/**
 * @brief class of root_model_object object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class root_model_object {
private:
  friend class odb::access;

protected:
  uint32_t m_version = 0;
#pragma db id auto
  uint32_t m_id = 0;
  std::string m_created_by = "anonymous";
  std::time_t m_created_at = std::time(nullptr);
  std::string m_updated_by = "anonymous";
  std::time_t m_updated_at = std::time(nullptr);

public:
  /**
   * @brief Construct a new root_model_object object
   *
   */
  root_model_object() = default;

  /**
   * @brief Set the version of object
   *
   * @param version The version of object
   */
  void setVersion(const uint32_t &version) { m_version = version; };

  /**
   * @brief Get the version of object
   *
   * @return const uint32_t& the version of object
   */
  const uint32_t &getVersion() const { return m_version; };

  /**
   * @brief Set the id of object
   *
   * @param id The id of object
   */
  void setId(const uint32_t &id) { m_id = id; };

  /**
   * @brief Get the id of object
   *
   * @return const uint32_t& the id of object
   */
  const uint32_t &getId() const { return m_id; };

  /**
   * @brief Set the created_by of object
   *
   * @param created_by The created_by of object
   */
  void setCreatedBy(const std::string &created_by) {
    m_created_by = created_by;
  };

  /**
   * @brief Get the created_by of object
   *
   * @return const std::string& the created_by of object
   */
  const std::string &getCreatedBy() const { return m_created_by; };

  /**
   * @brief Set the created_at of object
   *
   * @param created_at The created_at of object
   */
  void setCreatedAt(const std::time_t &created_at) {
    m_created_at = created_at;
  };

  /**
   * @brief Get the created_at of object
   *
   * @return const std::time_t& the created_at of object
   */
  const std::time_t &getCreatedAt() const { return m_created_at; };

  /**
   * @brief Set the updated_by of object
   *
   * @param updated_by The updated_by of object
   */
  void setUpdatedBy(const std::string &updated_by) {
    m_updated_by = updated_by;
  };

  /**
   * @brief Get the updated_by of object
   *
   * @return const std::string& the updated_by of object
   */
  const std::string &getUpdatedBy() const { return m_updated_by; };

  /**
   * @brief Set the updated_at of object
   *
   * @param updated_at The updated_at of object
   */
  void setUpdatedAt(const std::time_t &updated_at) {
    m_updated_at = updated_at;
  };

  /**
   * @brief Get the updated_at of object
   *
   * @return const std::time_t& the updated_at of object
   */
  const std::time_t &getUpdatedAt() const { return m_updated_at; };

  /**
   * @brief method to check if all fields of object are valid
   *
   * @return the error list of validation
   */
  std::list<boost::property_tree::ptree> getErrors() const {
    return std::list<boost::property_tree::ptree>();
  }

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
                            root_model_object, T>::value> * = nullptr>
  static std::unique_ptr<T>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<T> childObject = std::make_unique<T>();
    const boost::optional<uint32_t> id =
        property_tree.get_optional<uint32_t>("id");
    if (id) {
      childObject->setId(*id);
    }
    const boost::optional<uint32_t> version =
        property_tree.get_optional<uint32_t>("version");
    if (version) {
      childObject->setVersion(*version);
    }
    const boost::optional<std::string> created_by =
        property_tree.get_optional<std::string>("created_by");
    if (created_by) {
      childObject->setCreatedBy(*created_by);
    }
    const boost::optional<std::string> updated_by =
        property_tree.get_optional<std::string>("updated_by");
    if (updated_by) {
      childObject->setUpdatedBy(*updated_by);
    }
    return childObject;
  };

  /**
   * @brief method of pre create in database
   *
   * @param author The author of new object
   */
  void preCreate(const std::string &author) {
    setVersion(1);
    setCreatedBy(author);
    setCreatedAt(std::time(nullptr));
    setUpdatedBy(author);
    setUpdatedAt(std::time(nullptr));
  }

  /**
   * @brief method of pre update in database
   *
   * @param author The author of update of object
   */
  void preUpdate(const std::string &author) {
    setVersion(getVersion() + 1);
    setUpdatedBy(author);
    setUpdatedAt(std::time(nullptr));
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
  static std::string getObjectType() { return "root_model_object"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "root_model_objects"; }
};

#pragma db object(root_model_object)

#pragma db view object(root_model_object)
struct root_model_object_stat {
#pragma db column("count(" + root_model_object::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_ROOT_MODEL_OBJECT_HXX__

#ifdef ODB_COMPILER

#endif
