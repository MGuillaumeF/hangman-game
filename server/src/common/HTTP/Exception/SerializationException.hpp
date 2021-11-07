#ifndef __HTTP_SERIALIZATION_EXCEPTION_HPP__
#define __HTTP_SERIALIZATION_EXCEPTION_HPP__

#include <exception>
#include <string>

/**
 * Exception class for serialization error
 */
class SerializationException : public std::exception {
public:
  /** Constructor
   *  @param message The message exception
   */
  explicit SerializationException(const char * const message) : m_message(message) {}

  /** Constructor
   *  @param message The error message.
   */
  explicit SerializationException(const std::string &message)
      : m_message(message) {}

private:
  /** Error message.
   */
  std::string m_message = "Serialization error";
};
#endif // __HTTP_SERIALIZATION_EXCEPTION_HPP__
