#ifndef __LOGGERCONSOLE_H__
#define __LOGGERCONSOLE_H__

/**
 * Logger is mother class of LoggerConsole
 */
#include "Logger.hpp"
/**
 * The string include is to message parameters of LoggerConsole
 */
#include <string>

/**
 * class for log in console
 */
class LoggerConsole : public Logger {
private:
  /**
   * To write a log message
   * @param msg The message to print
   */
  void write(const std::string &msg);

  static LoggerConsole *s_pInstance;

  LoggerConsole(int level);

public:
  LoggerConsole *getInstance();
};

#endif // __LOGGERCONSOLE_H__