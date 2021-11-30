#ifndef __HTTP_CONFIGURATION_EXCEPTION_HPP__
#define __HTTP_CONFIGURATION_EXCEPTION_HPP__

#include <exception>
#include <string>

/**
 * Exception class for configuration error
 */
class ConfigurationException : public std::exception {
public:
  /** Constructor
   *  @param message The message exception
   */
  explicit ConfigurationException(const char *const message)
      : m_message(message) {}

  /** Constructor
   *  @param message The error message.
   */
  explicit ConfigurationException(const std::string_view &message)
      : m_message(message) {}

private:
  /** Error message.
   */
  std::string m_message = "Configuration error";
};
#endif
