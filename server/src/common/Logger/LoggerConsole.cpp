#include "LoggerConsole.hpp"

/**
 * The iostream include is to print messages in standard output or error output
 */
#include <iostream>

LoggerConsole *LoggerConsole::s_pInstance = nullptr;

LoggerConsole::LoggerConsole(int level) : Logger() { setLevel(level); }

LoggerConsole *LoggerConsole::getInstance() {
  if (s_pInstance == nullptr) {
    s_pInstance = new LoggerConsole(Logger::LEVEL::LINFO);
  }
  return s_pInstance;
}

/**
 * To write a log message
 * @param msg The message to print
 */
void LoggerConsole::write(const std::string &level, const std::string &theme,
                          const std::string &msg) {
  std::cout << '[' << level << "] - [" << theme << "] - " << msg << std::endl;
}