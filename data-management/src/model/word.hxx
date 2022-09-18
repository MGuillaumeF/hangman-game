#ifndef __MODEL_WORD_HXX__
#define __MODEL_WORD_HXX__

#include "./root_model_object.hxx"
#include <string>
#include <vector>

/**
 * @brief class of Words in model
 *
 */
#pragma db object
class word final : public root_model_object {
public:
  /**
   * @brief Construct a new word object
   *
   */
  word() = default;

  /**
   * @brief Get the Name of object
   *
   * @return const std::string&
   */
  const std::string &getName() const { return m_name; };

  /**
   * @brief Set the Name of object
   *
   * @param name The new name of object
   */
  void setName(const std::string &name) { m_name = name; };

  /**
   * @brief Get the Definitions object
   *
   * @return const std::vector<std::string>&
   */
  const std::vector<std::string> &getDefinitions() const {
    return m_definitions;
  };

  /**
   * @brief Set the Definition object
   *
   * @param definitions
   */
  void setDefinitions(const std::vector<std::string> &definitions) {
    m_definitions = definitions;
  };

  /**
   * @brief method to check if all fields of object are valid
   *
   * @return the error list of validation
   */
  std::list<boost::property_tree::ptree> getErrors() const {
    std::list<boost::property_tree::ptree> errors;
    if (getName().size() < 3) {
      boost::property_tree::ptree error;
      error.put("field", "name");
      error.put("message", "SIZE");
      errors.emplace_back(error);
    }
    return errors;
  }

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
  static word parse(const boost::property_tree::ptree &property_tree) {
    word parsedWord;
    parsedWord.setName(property_tree.get<std::string>("name"));
    return parsedWord;
  }

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "word"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "words"; }

private:
  friend class odb::access;

#pragma db unique options() options("CHECK(name != '')")
  std::string m_name;
#pragma db value_not_null unordered
  std::vector<std::string> m_definitions;
};

#pragma db object(word)

#pragma db view object(word)
struct word_stat {
#pragma db column("count(" + word::m_id + ")")
  std::size_t count;
};

#endif // __MODEL_WORD_HXX__
