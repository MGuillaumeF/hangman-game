#ifndef __HTTP_PARSING_EXCEPTION_HPP__
#define __HTTP_PARSING_EXCEPTION_HPP__

#include "./HangmanGameException.hpp"

/**
 * Exception class for parsing error
 */
class ParsingException : public HangmanGameException {
  /**
   * call parent constructor
   */
  using HangmanGameException::HangmanGameException;
  /**
   * parsing error message.
   */
  std::string m_message = "Parsing error";
};
#endif // __HTTP_PARSING_EXCEPTION_HPP__
