#ifndef __HTTP_SERIALIZATION_EXCEPTION_HPP__
#define __HTTP_SERIALIZATION_EXCEPTION_HPP__

#include "./HangmanGameException.hpp"

/**
 * Exception class for serialization error
 */
class SerializationException : public HangmanGameException {
public:
  /** Constructor
   *  @param message The message exception
   */
  explicit SerializationException(const char *const message)
      : HangmanGameException(message) {}

  /** Constructor
   *  @param message The error message.
   */
  explicit SerializationException(const std::string &message)
      : HangmanGameException(message) {}

private:
  /** Error message.
   */
  std::string m_message = "Serialization error";
};
#endif // __HTTP_SERIALIZATION_EXCEPTION_HPP__
