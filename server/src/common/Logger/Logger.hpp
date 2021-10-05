#ifndef __LOGGER_H__
#define __LOGGER_H__

/**
 * The string include is to message parameters of Logger
 */
#include <map>
#include <set>
#include <string>

using appender_t = void (*)(const std::string &);
typedef std::map<std::string, std::set<appender_t>> appendersByLevel;
typedef std::map<std::string, appendersByLevel> appendersByTheme;
/**
 * abstract class for generic definition of logger
 */
class Logger {
public:
private:
  static Logger *s_pInstance;
  static std::map<int, std::string> s_corresp;
  /**
   * The current log Level
   * @see LEVEL
   */
  int m_level = 1;
  /**
   * @brief appenders of logger for each theme and levels
   *
   */
  appendersByTheme m_appenders;
  /**
   * To write a log message
   * @param msg The message to print
   */
  void write(const std::string &level, const std::string &theme,
             const std::string &msg);

  Logger() {}

public:
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
  static Logger *getInstance();
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
  void debug(const std::string &theme, const std::string &msg);
  /**
   * To print info log
   * @param msg The message to print
   */
  void info(const std::string &theme, const std::string &msg);
  /**
   * To print warn log
   * @param msg The message to print
   */
  void warn(const std::string &theme, const std::string &msg);
  /**
   * To print error log
   * @param msg The message to print
   */
  void error(const std::string &theme, const std::string &msg);

  void addAppender(const int level, const std::string &theme, appender_t);

  std::string getLog(const std::string &level, const std::string &theme,
                     const std::string &msg);
  static void defaultOutAppender(const std::string &message);
  static void defaultErrAppender(const std::string &message);
};

#endif // __LOGGER_H__