#ifndef __MODEL_WORD_HXX__
#define __MODEL_WORD_HXX__

#include <string>

#include <odb/core.hxx>

/**
 * @brief class of Words in model
 *
 */
#pragma db object
class word {
public:
  /**
   * @brief Construct a new word object
   *
   */
  word() = default;

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
   * @brief Get the Value object
   *
   * @return const std::string&
   */
  const std::string &getValue() const { return m_value; };

  /**
   * @brief Set the Value object
   *
   * @param value
   */
  void setValue(const std::string &value) { m_value = value; };

  /**
   * @brief Get the Definition object
   *
   * @return const std::string&
   */
  const std::string &getDefinition() const { return m_definition; };

  /**
   * @brief Set the Definition object
   *
   * @param definition
   */
  void setDefinition(const std::string &definition) { m_definition = definition; };

  /**
   * @brief method to check if all fields of word are valid
   *
   * @return true The content of word object is valid
   * @return false The content of word object is invalid
   */
  static bool isValid(const word &wrd) { return wrd.getValue().size() > 3 && wrd.getDefinition().size() > 3; }

private:
  friend class odb::access;

#pragma db id auto
  uint32_t m_id;

#pragma db options() options("CHECK(value != '')")
  std::string m_value;
#pragma db options() options("CHECK(definition != '')")
  std::string m_definition;
};

#pragma db object(word)

#pragma db view object(word)
struct word_stat {
#pragma db column("count(" + word::m_id + ")")
  std::size_t count;
};

#endif // __MODEL_WORD_HXX__