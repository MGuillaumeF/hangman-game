#include "LoggerConsole.hpp"

/**
 * The iostream include is to print messages in standard output or error output
 */
#include <iostream>

/**
 * To write a log message
 * @param msg The message to print
 */
void LoggerConsole::write(const std::string &msg) {
  std::cout << msg << std::endl;
}