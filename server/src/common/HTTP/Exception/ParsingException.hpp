#ifndef __HTTP_PARSING_EXCEPTION_HPP__
#define __HTTP_PARSING_EXCEPTION_HPP__

#include "./HangmanGameException.hpp"

/**
 * Exception class for parsing error
 */
class ParsingException : public HangmanGameException {
public:
  /** Constructor
   *  @param message The message exception
   */
  explicit ParsingException(const char * const message) : HangmanGameException(message) {}

  /** Constructor
   *  @param message The error message.
   */
  explicit ParsingException(const std::string &message) : HangmanGameException(message) {}

private:
  /** Error message.
   */
  std::string m_message = "Parsing error";
};
#endif // __HTTP_PARSING_EXCEPTION_HPP__
