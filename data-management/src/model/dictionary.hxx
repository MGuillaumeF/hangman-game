#ifndef __MODEL_DICTIONARY_HXX__
#define __MODEL_DICTIONARY_HXX__

#include "./root_model_object.hxx"
#include "./word.hxx"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief class of Dictionary in model
 *
 */
#pragma db object
class dictionary final : public root_model_object {
public:
  /**
   * @brief Construct a new dictionary object
   *
   */
  dictionary() = default;

  /**
   * @brief Get the name of dictionary object
   *
   * @return const std::string&
   */
  const std::string &getName() const { return m_name; };

  /**
   * @brief Set the name of dictionary object
   *
   * @param name
   */
  void setName(const std::string &name) { m_name = name; };

  /**
   * @brief Get the country_code of dictionary object
   *
   * @return const std::string&
   */
  const std::string &getCountryCode() const { return m_country_code; };

  /**
   * @brief Set the country_code of dictionary object
   *
   * @param country_code
   */
  void setCountryCode(const std::string &country_code) {
    m_country_code = country_code;
  };

  /**
   * @brief Get words of dictionary object
   *
   * @return const std::vector<std::shared_ptr<word>>&
   */
  const std::vector<std::shared_ptr<word>> &getWords() const {
    return m_words;
  };

  /**
   * @brief Set words of group object
   *
   * @param words
   */
  void setWords(const std::vector<std::shared_ptr<word>> &words) {
    m_words = words;
  };

  /**
   * @brief method to check if all fields of object are valid
   *
   * @return the error list of validation
   */
  const std::list<boost::property_tree::ptree> getErrors() {
    std::list<boost::property_tree::ptree> errors;
    if (getName().size() < 3) {
      boost::property_tree::ptree error;
      error.put("field", "name");
      error.put("message", "SIZE");
      errors.emplace_back(error);
    }
    if (getCountryCode().size() < 3) {
      boost::property_tree::ptree error;
      error.put("field", "country_code");
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
  static dictionary parse(const boost::property_tree::ptree &property_tree) {
    dictionary parsedDictionary =
        root_model_object::parse<dictionary>(property_tree);
    parsedDictionary.setName(property_tree.get<std::string>("name"));
    parsedDictionary.setCountryCode(
        property_tree.get<std::string>("country_code"));
    return parsedDictionary;
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

private:
  friend class odb::access;

#pragma db unique options() options("CHECK(name != '')")
  std::string m_name;
#pragma db unique options() options("CHECK(country_code != '')")
  std::string m_country_code;
#pragma db value_not_null unordered
  std::vector<std::shared_ptr<word>> m_words;
};

#pragma db object(dictionary)

#pragma db view object(dictionary)
struct dictionary_stat {
#pragma db column("count(" + dictionary::m_id + ")")
  std::size_t count;
};

#endif // __MODEL_DICTIONARY_HXX__
