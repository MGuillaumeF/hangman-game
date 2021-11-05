#ifndef __LOGGER_H__
#define __LOGGER_H__

/**
 * The string include is to message parameters of Logger
 */
#include <map>
#include <set>
#include <string>

/**
 * All levels available for logs
 */
enum class ELogLevel {
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
public:
  using appender_t = void (*)(const std::string &);
  using appendersByLevel = std::map<std::string, std::set<appender_t>>;
  using appendersByTheme = std::map<std::string, appendersByLevel>;

private:
  static Logger *s_pInstance;
  static std::map<ELogLevel, std::string> s_corresp;
  /**
   * The current log Level
   * @see LEVEL
   */
  ELogLevel m_level = ELogLevel::LINFO;
  /**
   * @brief appenders of logger for each theme and levels
   *
   */
  appendersByTheme m_appenders;
  /**
   * To write a log message
   * @param level The level of message
   * @param theme The theme of message
   * @param msg The message to print
   */
  void write(const std::string &level, const std::string &theme,
             const std::string &msg);

  /**
   * @brief Construct a new Logger object
   *
   */
  Logger() = default;

public:
  /**
   * @brief Get instance of singleton logger
   *
   * @return Logger* address of logger instance
   */
  static Logger *getInstance();
  /**
   * To get the log level of Logger
   * @see LEVEL
   */
  ELogLevel getLevel() const;
  /**
   * To set the log level of Logger
   * @param level The new level of logger
   * @see LEVEL
   */
  void setLevel(ELogLevel level);
  /**
   * To print debug log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void debug(const std::string &theme, const std::string &msg);
  /**
   * To print info log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void info(const std::string &theme, const std::string &msg);
  /**
   * To print warn log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void warn(const std::string &theme, const std::string &msg);
  /**
   * To print error log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void error(const std::string &theme, const std::string &msg);

  /**
   * To write a log message
   * @param level The level of message
   * @param theme The theme of message
   * @param appender The tracer function to manage printing of message
   */
  void addAppender(const ELogLevel level, const std::string &theme,
                   appender_t tracer);

  /**
   * Function to get formated message
   * @param level The level of message
   * @param theme The theme of message
   * @param msg The message body
   */
  std::string getLog(const std::string &level, const std::string &theme,
                     const std::string &msg) const;
  /**
   * Function of default std:cout appender
   * @param message The message to print
   */
  static void defaultOutAppender(const std::string &message);
  /**
   * Function of default std:cerr appender
   * @param message The message to print
   */
  static void defaultErrAppender(const std::string &message);
};

#endif // __LOGGER_H__
