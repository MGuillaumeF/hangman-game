#include "LoggerFile.hpp"
/**
 * The fstream include is to print messages in file
 */
#include <fstream>

LoggerFile *LoggerFile::s_pInstance = nullptr;

/**
 * Contructor of Logger
 */
LoggerFile::LoggerFile() {}

LoggerFile *LoggerFile::getInstance() {
  if (s_pInstance == nullptr) {
    s_pInstance = new LoggerFile();
  }
  return s_pInstance;
}

/**
 * To write a log message
 * @param msg The message to print
 */
void LoggerFile::write(const std::string &level, const std::string &theme,
                       const std::string &msg) {
  std::fstream fs;
  fs.open("./logs/logfile.log",
          std::fstream::in | std::fstream::out | std::fstream::app);
  fs << '[' << level << "] - [" << theme << "] - " << msg << std::endl;
  fs.close();
}
/**
 * Destructor of Logger
 */
LoggerFile::~LoggerFile() {}