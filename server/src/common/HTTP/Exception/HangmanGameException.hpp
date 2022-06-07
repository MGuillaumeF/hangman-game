#ifndef __HANGMAN_GAME_EXCEPTION_HPP__
#define __HANGMAN_GAME_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <string_view>

/**
 * Exception class for all hangman game exception
 */
class HangmanGameException : public std::exception {
public:
  /** Constructor
   *  @param message The message exception
   */
  explicit HangmanGameException(const char *const message)
      : m_message(message) {}

  /** Constructor
   *  @param message The error message.
   */
  explicit HangmanGameException(const std::string_view &message)
      : m_message(message) {}

  const char *what() const noexcept override { return m_message.c_str(); }

private:
  /** Error message.
   */
  std::string m_message = "Hangman Game exception";
};
#endif
