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
   * @brief method to check if all fields of group are valid
   *
   * @return true The content of group object is valid
   * @return false The content of group object is invalid
   */
  static bool isValid(const dictionary &dict) {
    return dict.getName().size() > 3;
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
    dictionary parsedDictionary;
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

#pragma db options() options("CHECK(name != '')")
  std::string m_name;
#pragma db value_not_null unordered
  std::vector<std::weak_ptr<dictionary>> m_dictionaries;
};

#pragma db object(dictionary)

#pragma db view object(dictionary)
struct dictionary_stat {
#pragma db column("count(" + dictionary::m_id + ")")
  std::size_t count;
};

#endif // __MODEL_DICTIONARY_HXX__
