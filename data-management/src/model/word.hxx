
/**
 * @brief word.hxx DO NOT MODIFY THIS FILE, this file is a generated model class
 */

#ifndef __GENERATED_MODEL_OBJECT_WORD_HXX__
#define __GENERATED_MODEL_OBJECT_WORD_HXX__

#include "./dictionary.hxx"
#include "./root_model_object.hxx"

#include <memory>
#include <string>
#include <vector>

class dictionary;

/**
 * @brief class of word object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class word final : public root_model_object {
private:
  friend class odb::access;
  std::string m_name;
  std::vector<std::string> m_definitions;
#pragma db not_null
  std::shared_ptr<dictionary> m_dictionary;

public:
  /**
   * @brief Construct a new word object
   *
   */
  word() = default;

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
   * @brief Set the definitions of object
   *
   * @param definitions The definitions of object
   */
  void setDefinitions(const std::vector<std::string> &definitions) {
    m_definitions = definitions;
  };

  /**
   * @brief Get the definitions of object
   *
   * @return const std::vector<std::string>& the definitions of object
   */
  const std::vector<std::string> &getDefinitions() const {
    return m_definitions;
  };

  /**
   * @brief Set the dictionary of object
   *
   * @param dictionary The dictionary of object
   */
  void setDictionary(const std::shared_ptr<dictionary> &dictionary) {
    m_dictionary = dictionary;
  };

  /**
   * @brief Get the dictionary of object
   *
   * @return const std::shared_ptr<dictionary>& the dictionary of object
   */
  const std::shared_ptr<dictionary> &getDictionary() const {
    return m_dictionary;
  };

  /**
   * @brief method to extract object from property tree
   *
   * @return The word found
   */
  static std::unique_ptr<word>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<word> parsedObject =
        root_model_object::parse<word>(property_tree);
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
  static std::string getObjectType() { return "word"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "words"; }
};

#pragma db object(word)

#pragma db view object(word)
struct word_stat {
#pragma db column("count(" + word::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_WORD_HXX__

#ifdef ODB_COMPILER

#endif
