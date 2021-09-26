
#include "Logger.hpp"

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
void Logger::debug(const std::string &msg) {
  if (m_level == LEVEL::LDEBUG) {
    write(msg);
  }
}
/**
 * To print info log
 * @param msg The message to print
 */
void Logger::info(const std::string &msg) {
  if (m_level <= LEVEL::LINFO) {
    write(msg);
  }
}
/**
 * To print warn log
 * @param msg The message to print
 */
void Logger::warn(const std::string &msg) {
  if (m_level <= LEVEL::LWARN) {
    write(msg);
  }
}
/**
 * To print error log
 * @param msg The message to print
 */
void Logger::error(const std::string &msg) {
  if (m_level <= LEVEL::LERROR) {
    write(msg);
  }
}