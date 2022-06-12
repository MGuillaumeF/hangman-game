#ifndef __HTTP_CONFIGURATION_EXCEPTION_HPP__
#define __HTTP_CONFIGURATION_EXCEPTION_HPP__

#include "./HangmanGameException.hpp"

/**
 * Exception class for configuration error
 */
class ConfigurationException : public HangmanGameException {
public:
  /** Constructor
   *  @param message The message exception
   */
  explicit ConfigurationException(const char *const message)
      : HangmanGameException(message) {
    using HangmanGameException::HangmanGameException;
  }

  /** Constructor
   *  @param message The error message.
   */
  explicit ConfigurationException(const std::string_view &message)
      : HangmanGameException(message) {
    using HangmanGameException::HangmanGameException;
  }

private:
  /** Error message.
   */
  std::string m_message = "Configuration error";
};
#endif
