
#include "Logger.hpp"
#include <fstream>
#include <iostream>
#include <streambuf>

std::unique_ptr<Logger> Logger::s_pInstance = std::make_unique<Logger>();

std::map<ELogLevel, std::string, std::less<>> Logger::s_corresp = {
    {ELogLevel::LDEBUG, "DEBUG"},
    {ELogLevel::LINFO, "INFO"},
    {ELogLevel::LWARN, "WARN"},
    {ELogLevel::LERROR, "ERROR"}};

std::map<std::string, std::unique_ptr<std::ofstream>, std::less<>>
    Logger::s_files = {};

/**
 * @brief Get instance of singleton logger
 *
 * @return Logger* address of logger instance
 */
std::unique_ptr<Logger> &Logger::getInstance() { return s_pInstance; }

/**
 * @brief To get the log level of Logger
 *
 * @return int log level
 */
ELogLevel Logger::getLevel() const { return m_level; }
/**
 * To set the log level of Logger
 * @param level The new level of logger
 * @see LEVEL
 */
void Logger::setLevel(const ELogLevel &level) {
  if (level >= ELogLevel::LDEBUG && level <= ELogLevel::LERROR) {
    m_level = level;
  }
}
/**
 * To print debug log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::debug(const std::string &theme, const std::string &msg) const {
  if (m_level == ELogLevel::LDEBUG) {
    write("DEBUG", theme, msg);
  }
}
/**
 * To print info log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::info(const std::string &theme, const std::string &msg) const {
  if (m_level <= ELogLevel::LINFO) {
    write("INFO", theme, msg);
  }
}
/**
 * To print warn log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::warn(const std::string &theme, const std::string &msg) const {
  if (m_level <= ELogLevel::LWARN) {
    write("WARN", theme, msg);
  }
}
/**
 * To print error log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::error(const std::string &theme, const std::string &msg) const {
  if (m_level <= ELogLevel::LERROR) {
    write("ERROR", theme, msg);
  }
}

/**
 * Function to get formated message
 * @param level The level of message
 * @param theme The theme of message
 * @param msg The message body
 */
std::string Logger::getLog(const std::string &level, const std::string &theme,
                           const std::string &msg) {
  std::string l_message = "[" + level + "] - [" + theme + "] - " + msg;
  return l_message;
}

/**
 * Function of default std:cout appender
 * @param message The message to print
 */
void Logger::defaultOutAppender(const std::string &message) {
  std::cout << message << std::endl;
}

/**
 * Function of default std:cerr appender
 * @param message The message to print
 */
void Logger::defaultErrAppender(const std::string &message) {
  std::cerr << message << std::endl;
}

/**
 * @brief Get the Logger File object
 *
 * @param filename The name of logger file
 * @return std::fstream The stream of file
 */
std::unique_ptr<std::ofstream> &
Logger::getLoggerFile(const std::string &filename) {
  if (!Logger::s_files.contains(filename)) {
    Logger::s_files[filename] =
        std::make_unique<std::ofstream>("./logs/" + filename + ".log");
  }
  return Logger::s_files[filename];
}

/**
 * To write a log message
 * @param level The level of message
 * @param theme The theme of message
 * @param appender The tracer function to manage printing of message
 */
void Logger::addAppender(const ELogLevel &level, const std::string &theme,
                         const appender_t &appender) {
  if (m_appenders.contains(theme)) {
    if (m_appenders[theme].contains(Logger::s_corresp[level])) {
      m_appenders[theme][Logger::s_corresp[level]].insert(appender);
    } else {
      m_appenders[theme][Logger::s_corresp[level]] =
          std::set<appender_t>{appender};
    }
  } else {
    m_appenders[theme] =
        std::map<std::string, std::set<appender_t>, std::less<>>{
            {Logger::s_corresp[level], std::set<appender_t>{appender}}};
  }
}

/**
 * To write a log message
 * @param msg The message to print
 */
void Logger::write(const std::string &level, const std::string &theme,
                   const std::string &msg) const {
  if (m_appenders.contains(theme) && m_appenders.at(theme).contains(level)) {
    const std::string message = getLog(level, theme, msg);
    const std::set<appender_t> appenders = m_appenders.at(theme).at(level);
    for (const appender_t &appender : appenders) {
      (*appender)(message);
    }
  }
}
