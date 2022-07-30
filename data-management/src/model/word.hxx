#ifndef __MODEL_WORD_HXX__
#define __MODEL_WORD_HXX__

#include "./root_model_object.hxx"
#include <string>

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
  void setValue(const std::string &name) { m_name = name; };

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
  void setDefinitions(const std::string<std::string> &definitions) {
    m_definitions = definitions;
  };

  /**
   * @brief method to check if all fields of word are valid
   *
   * @return true The content of word object is valid
   * @return false The content of word object is invalid
   */
  static bool isValid(const word &wrd) {
    return wrd.getValue().size() > 3 && wrd.getDefinitions().size() > 1;
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
    parsedWord.setName(property_tree.get<std::string>("name");
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

#pragma db options() options("CHECK(value != '')")
  std::string m_value;
#pragma db options() options("CHECK(definition != '')")
  std::vector<std::string> m_definitions;
};

#pragma db object(word)

#pragma db view object(word)
struct word_stat {
#pragma db column("count(" + word::m_id + ")")
  std::size_t count;
};

#endif // __MODEL_WORD_HXX__
