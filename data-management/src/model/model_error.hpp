#ifndef __MODEL_ERROR_HPP__
#define __MODEL_ERROR_HPP__

#include <boost/property_tree/ptree.hpp>
#include <map>
#include <string>

enum class model_error_code {
  MAX_LENGTH,
  MAX_SIZE,
  MIN_LENGTH,
  MIN_SIZE,
  MUST_BE_NULL,
  MUST_NOT_NULL,
  PATTERN
};

const std::map<model_error_code, std::string> model_error_code_text = {
    {model_error_code::MAX_LENGTH, "MAX_LENGTH"},
    {model_error_code::MAX_SIZE, "MAX_SIZE"},
    {model_error_code::MIN_LENGTH, "MIN_LENGTH"},
    {model_error_code::MIN_SIZE, "MIN_SIZE"},
    {model_error_code::MUST_BE_NULL, "MUST_BE_NULL"},
    {model_error_code::MUST_NOT_NULL, "MUST_NOT_NULL"},
    {model_error_code::PATTERN, "PATTERN"}};

class model_error {
  std::string m_attribute_name;
  model_error_code m_code;
  std::string m_message;

public:
  model_error(std::string attribute_name, model_error_code code,
              std::string message)
      : m_attribute_name(attribute_name), m_code(code), m_message(message) {}

  std::string getAttributeName() const { return m_attribute_name; }

  model_error_code getCode() const { return m_code; }

  std::string getCodeText() const { return model_error_code_text.at(m_code); }

  std::string getMessage() const { return m_message; }

  /**
   * method to convert error to ptree object
   */
  boost::property_tree::ptree toPtree() const {
    boost::property_tree::ptree ptree;
    ptree.put("attribute", getAttributeName());
    ptree.put("code", getCodeText());
    ptree.put("message", getMessage());
    return ptree;
  }
};

#endif
