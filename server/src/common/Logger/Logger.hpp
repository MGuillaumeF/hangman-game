#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

/**
 * The string include is to message parameters of Logger
 */
#include <functional>
#include <map>
#include <memory>
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
  using appendersByLevel =
      std::map<std::string, std::set<appender_t>, std::less<>>;
  using appendersByTheme = std::map<std::string, appendersByLevel, std::less<>>;

  /**
   * @brief Construct a new Logger object
   *
   */
  Logger() = default;

  /**
   * @brief Destructor Logger object
   *
   */
  ~Logger() = default;

  /**
   * @brief Get instance of singleton logger
   *
   * @return Logger* address of logger instance
   */
  static std::unique_ptr<Logger> &getInstance();
  /**
   * To get the log level of Logger
   * @see LEVEL
   */
  [[nodiscard]] ELogLevel getLevel() const;
  /**
   * To set the log level of Logger
   * @param level The new level of logger
   * @see LEVEL
   */
  void setLevel(const ELogLevel &level);
  /**
   * To print debug log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void debug(const std::string &theme, const std::string &msg) const;
  /**
   * To print info log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void info(const std::string &theme, const std::string &msg) const;
  /**
   * To print warn log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void warn(const std::string &theme, const std::string &msg) const;
  /**
   * To print error log
   * @param theme The theme of message
   * @param msg The message to print
   */
  void error(const std::string &theme, const std::string &msg) const;

  /**
   * To write a log message
   * @param level The level of message
   * @param theme The theme of message
   * @param appender The tracer function to manage printing of message
   */
  void addAppender(const ELogLevel &level, const std::string &theme,
                   const appender_t &appender);

  /**
   * Function to get formated message
   * @param level The level of message
   * @param theme The theme of message
   * @param msg The message body
   */
  static std::string getLog(const std::string &level, const std::string &theme,
                            const std::string &msg);
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

  /**
   * @brief Get the Logger File object
   *
   * @param filename The name of logger file
   * @return std::fstream The stream of file
   */
  static std::unique_ptr<std::ofstream> &
  getLoggerFile(const std::string &filename);

  Logger(const Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger &operator=(Logger &&) = delete;

private:
  static std::unique_ptr<Logger> s_pInstance;
  static std::map<ELogLevel, std::string, std::less<>> s_corresp;
  static std::map<std::string, std::unique_ptr<std::ofstream>, std::less<>>
      s_files;
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
             const std::string &msg) const;
};

#endif // __LOGGER_H__
