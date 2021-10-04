
#include "Logger.hpp"
#include <iostream>
#include <streambuf>

Logger *Logger::s_pInstance = nullptr;

Logger *Logger::getInstance() {
  if (s_pInstance == nullptr) {
    s_pInstance = new Logger();
  }
  return s_pInstance;
}

/**
 * To get the log level of Logger
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
 * @param msg The message to print
 */
void Logger::debug(const std::string &theme, const std::string &msg) {
  if (m_level == LEVEL::LDEBUG) {
    write("DEBUG", theme, msg);
  }
}
/**
 * To print info log
 * @param msg The message to print
 */
void Logger::info(const std::string &theme, const std::string &msg) {
  if (m_level <= LEVEL::LINFO) {
    write("INFO", theme, msg);
  }
}
/**
 * To print warn log
 * @param msg The message to print
 */
void Logger::warn(const std::string &theme, const std::string &msg) {
  if (m_level <= LEVEL::LWARN) {
    write("WARN", theme, msg);
  }
}
/**
 * To print error log
 * @param msg The message to print
 */
void Logger::error(const std::string &theme, const std::string &msg) {
  if (m_level <= LEVEL::LERROR) {
    write("ERROR", theme, msg);
  }
}

std::string Logger::getLog(const std::string &level, const std::string &theme,
                           const std::string &msg) {
  std::string l_message = "[" + level + "] - [" + theme + "] - " + msg;
  return l_message;
}

void Logger::defaultOutAppender(const std::string &message) {
  std::cout << message << std::endl;
}
void Logger::defaultErrAppender(const std::string &message) {
  std::cerr << message << std::endl;
}

void Logger::addAppender(const int level, const std::string &theme,
                         appender_t appender) {
  std::map<int, std::string> corresp = {
      {0, "DEBUG"}, {1, "INFO"}, {2, "WARN"}, {3, "ERROR"}};
  if (m_appenders.find(theme) != m_appenders.end()) {
    if (m_appenders[theme].find(corresp[level]) != m_appenders[theme].end()) {
      m_appenders[theme][corresp[level]].insert(appender);
    } else {
      m_appenders[theme][corresp[level]] = std::set<appender_t>{appender};
    }
  } else {
    m_appenders[theme] = std::map<std::string, std::set<appender_t>>{
        {corresp[level], std::set<appender_t>{appender}}};
  }
}

/**
 * To write a log message
 * @param msg The message to print
 */
void Logger::write(const std::string &level, const std::string &theme,
                   const std::string &msg) {
  const std::string message = getLog(level, theme, msg);
  const std::set<appender_t> appenders = m_appenders[theme][level];
  for (const appender_t &appender : appenders) {
    (*appender)(message);
  }
}