#ifndef __HTTP_CONFIGURATION_EXCEPTION_HPP__
#define __HTTP_CONFIGURATION_EXCEPTION_HPP__

#include "./HangmanGameException.hpp"

/**
 * Exception class for configuration error
 */
class ConfigurationException : public HangmanGameException {
  using HangmanGameException::HangmanGameException;

private:
  /** Error message.
   */
  std::string m_message = "Configuration error";
};
#endif
