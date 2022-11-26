
/**
 * @brief dictionary.hxx DO NOT MODIFY THIS FILE, this file is a generated model
 * class
 */

#ifndef __GENERATED_MODEL_OBJECT_DICTIONARY_HXX__
#define __GENERATED_MODEL_OBJECT_DICTIONARY_HXX__

#include "./root_model_object.hxx"

#include <memory>
#include <string>
#include <vector>

class word;

/**
 * @brief class of dictionary object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class dictionary final : public root_model_object {
private:
  friend class odb::access;
  std::string m_name;
  std::string m_country_code;
#pragma db value_not_null inverse(m_dictionary)
  std::vector<std::weak_ptr<word>> m_words;

public:
  /**
   * @brief Construct a new dictionary object
   *
   */
  dictionary() = default;

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
   * @brief Set the country_code of object
   *
   * @param country_code The country_code of object
   */
  void setCountryCode(const std::string &country_code) {
    m_country_code = country_code;
  };

  /**
   * @brief Get the country_code of object
   *
   * @return const std::string& the country_code of object
   */
  const std::string &getCountryCode() const { return m_country_code; };

  /**
   * @brief Set the words of object
   *
   * @param words The words of object
   */
  void setWords(const std::vector<std::weak_ptr<word>> &words) {
    m_words = words;
  };

  /**
   * @brief Get the words of object
   *
   * @return const std::vector<std::weak_ptr<word>>& the words of object
   */
  const std::vector<std::weak_ptr<word>> &getWords() const { return m_words; };

  /**
   * @brief method to extract object from property tree
   *
   * @return The dictionary found
   */
  static std::unique_ptr<dictionary>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<dictionary> parsedObject =
        root_model_object::parse<dictionary>(property_tree);
    const boost::optional<std::string> name =
        property_tree.get_optional<std::string>("name");
    if (name) {
      parsedObject->setName(*name);
    }
    const boost::optional<std::string> country_code =
        property_tree.get_optional<std::string>("country_code");
    if (country_code) {
      parsedObject->setCountryCode(*country_code);
    }

    return parsedObject;
  }

  /**
   * @brief method to check if all fields of object are valid
   *
   * @return the error vector of validation
   */
  std::vector<model_error> getErrors() const {
    std::vector<model_error> errors;
    // TODO add implementation
    return errors;
  }

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "dictionary"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "dictionaries"; }
};

#pragma db object(dictionary)

#pragma db view object(dictionary)
struct dictionary_stat {
#pragma db column("count(" + dictionary::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_DICTIONARY_HXX__

#ifdef ODB_COMPILER
#include "./word.hxx"
#endif
