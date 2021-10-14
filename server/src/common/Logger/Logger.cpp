
#include "Logger.hpp"
#include <iostream>
#include <streambuf>

Logger *Logger::s_pInstance = nullptr;

std::map<int, std::string> Logger::s_corresp = {
    {0, "DEBUG"}, {1, "INFO"}, {2, "WARN"}, {3, "ERROR"}};

/**
 * @brief Get instance of singleton logger
 *
 * @return Logger* address of logger instance
 */
Logger *Logger::getInstance() {
  if (s_pInstance == nullptr) {
    s_pInstance = new Logger();
  }
  return s_pInstance;
}

/**
 * @brief To get the log level of Logger
 *
 * @return int log level
 */
int Logger::getLevel() { return m_level; }
/**
 * To set the log level of Logger
 * @param level The new level of logger
 * @see LEVEL
 */
void Logger::setLevel(int level) {
  if (level >= LEVEL::LDEBUG && level <= LEVEL::LERROR) {
    m_level = level;
  }
}
/**
 * To print debug log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::debug(const std::string &theme, const std::string &msg) {
  if (m_level == LEVEL::LDEBUG) {
    write("DEBUG", theme, msg);
  }
}
/**
 * To print info log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::info(const std::string &theme, const std::string &msg) {
  if (m_level <= LEVEL::LINFO) {
    write("INFO", theme, msg);
  }
}
/**
 * To print warn log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::warn(const std::string &theme, const std::string &msg) {
  if (m_level <= LEVEL::LWARN) {
    write("WARN", theme, msg);
  }
}
/**
 * To print error log
 * @param theme The theme of message
 * @param msg The message to print
 */
void Logger::error(const std::string &theme, const std::string &msg) {
  if (m_level <= LEVEL::LERROR) {
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
 * To write a log message
 * @param level The level of message
 * @param theme The theme of message
 * @param appendet The tracer function to manage printing of message
 */
void Logger::addAppender(const int level, const std::string &theme,
                         appender_t appender) {
  if (m_appenders.find(theme) != m_appenders.end()) {
    if (m_appenders[theme].find(Logger::s_corresp[level]) !=
        m_appenders[theme].end()) {
      m_appenders[theme][Logger::s_corresp[level]].insert(appender);
    } else {
      m_appenders[theme][Logger::s_corresp[level]] =
          std::set<appender_t>{appender};
    }
  } else {
    m_appenders[theme] = std::map<std::string, std::set<appender_t>>{
        {Logger::s_corresp[level], std::set<appender_t>{appender}}};
  }
}

/**
 * To write a log message
 * @param msg The message to print
 */
void Logger::write(const std::string &level, const std::string &theme,
                   const std::string &msg) {
  if ((m_appenders.find(theme) != m_appenders.end()) &&
      (m_appenders[theme].find(level) != m_appenders[theme].end())) {
    const std::string message = getLog(level, theme, msg);
    const std::set<appender_t> appenders = m_appenders[theme][level];
    for (const appender_t &appender : appenders) {
      (*appender)(message);
    }
  }
}
