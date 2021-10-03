#ifndef __LOGGERFILE_H__
#define __LOGGERFILE_H__

/**
 * Logger is mother class of LoggerConsole
 */
#include "Logger.hpp"
/**
 * The string include is to message parameters of LoggerConsole
 */
#include <string>

/**
 * Class for log in file
 */
class LoggerFile : public Logger {
private:
  /**
   * To write a log message
   * @param msg The message to print
   */
  void write(const std::string &level, const std::string &theme,
             const std::string &msg);

public:
  /**
   * Constructor of Logger
   */
  LoggerFile();
  /**
   * Destructor of Logger
   */
  ~LoggerFile();
  static LoggerFile *s_pInstance;
  static LoggerFile *getInstance();
};

#endif // __LOGGERFILE_H__