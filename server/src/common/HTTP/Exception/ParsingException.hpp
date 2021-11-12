#ifndef __HTTP_PARSING_EXCEPTION_HPP__
#define __HTTP_PARSING_EXCEPTION_HPP__

#include <exception>
#include <string>

/**
 * Exception class for parsing error
 */
class ParsingException : public std::exception {
public:
  /** Constructor
   *  @param message The message exception
   */
  explicit ParsingException(const char * const message) : m_message(message) {}

  /** Constructor
   *  @param message The error message.
   */
  explicit ParsingException(const std::string &message) : m_message(message) {}

private:
  /** Error message.
   */
  std::string m_message = "Parsing error";
};
#endif // __HTTP_PARSING_EXCEPTION_HPP__
