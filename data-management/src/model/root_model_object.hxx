#ifndef __ROOT_MODEL_OBJECT_HXX__
#define __ROOT_MODEL_OBJECT_HXX__

#include <string>

#include <odb/core.hxx>

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
   * @brief method to check if all fields of object are valid
   *
   * @return true The content of object is valid
   * @return false The content of object is invalid
   */
  static bool isValid(const root_model_object &rootModelObject);

private:
  friend class odb::access;

#pragma db id auto
  uint32_t m_id;
  uint32_t m_version;
  std::string m_created_by;
  uint32_t m_created_at;
  std::string m_updated_by;
  uint32_t m_updated_at;
};

#pragma db object(root_model_object)

#pragma db view object(root_model_object)
struct root_model_object_stat {
#pragma db column("count(" + root_model_object::m_id + ")")
  std::size_t count;
};

#endif // ROOT_MODEL_OBJECT_HXX
