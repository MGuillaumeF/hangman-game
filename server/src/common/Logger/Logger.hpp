#ifndef __LOGGER_H__
#define __LOGGER_H__

/**
 * The string include is to message parameters of Logger
 */
#include <string>

/**
 * All levels available for logs
 */
enum LEVEL {
  /**
   * The **debug** level
   * Level of Logger for dev mode
   * To print debug message with Logger to help dev
   * value 0
   */
  LDEBUG = 0,
  /**
   * The **information** level
   * Default level of Logger for production mode
   * To print information, normal message with Logger
   * value 1
   */
  LINFO = 1,
  /**
   * The **warnning** level
   * To print warnning, important message with Logger
   * value 2
   */
  LWARN = 2,
  /**
   * The **error** level
   * To print error, very important message with Logger
   * value 3
   */
  LERROR = 3
};

/**
 * abstract class for generic definition of logger
 */
class Logger {
private:
  /**
   * The current log Level
   * @see LEVEL
   */
  int m_level = 1;

  /**
   * To write a log message
   * @param msg The message to print
   */
  virtual void write(const std::string &msg) = 0;

public:
  /**
   * To get the log level of Logger
   * @see LEVEL
   */
  int getLevel();
  /**
   * To set the log level of Logger
   * @param level The new level of logger
   * @see LEVEL
   */
  void setLevel(int level);
  /**
   * To print debug log
   * @param msg The message to print
   */
  void debug(const std::string &msg);
  /**
   * To print info log
   * @param msg The message to print
   */
  void info(const std::string &msg);
  /**
   * To print warn log
   * @param msg The message to print
   */
  void warn(const std::string &msg);
  /**
   * To print error log
   * @param msg The message to print
   */
  void error(const std::string &msg);
};

#endif // __LOGGER_H__