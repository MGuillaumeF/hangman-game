#include "./common/Logger/Logger.hpp"

#include "./api/HttpTokenEndpoint.hpp"
#include "./common/HTTP/Configuration/ConfigurationServer.hpp"
#include "./common/HTTP/LocationEndpoint.hpp"
#include "./common/HTTP/Server.hpp"
#include "./common/HTTP/Session.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

#include <boost/lambda/lambda.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "./config.hpp"

/**
 * To write a log message
 * @param filename The name of log file
 * @param message The message to print
 */
void appenderFile(const std::string &filename, const std::string &message) {
  const std::unique_ptr<std::ofstream> &file = Logger::getLoggerFile(filename);
  file->write(message.c_str(), message.size());
  file->write("\n", 1);
}

/**
 * To write a log message
 * @param message The message to print
 */
void appenderAuditFile(const std::string &message) {
  // global streams for files loggers
  appenderFile("AUDIT", message);
}

/**
 * To write a log message
 * @param message The message to print
 */
void appenderAccessFile(const std::string &message) {
  // light http access logs
  appenderFile("ACCESS", message);
}

void loadLoggerConfiguration() {
  const std::unique_ptr<Logger> &logger = Logger::getInstance();

  logger->addAppender(ELogLevel::LDEBUG, "HTTP_ACCESS",
                      Logger::defaultOutAppender);
  logger->addAppender(ELogLevel::LINFO, "HTTP_ACCESS", appenderAccessFile);
  logger->addAppender(ELogLevel::LWARN, "HTTP_ACCESS", appenderAccessFile);
  logger->addAppender(ELogLevel::LERROR, "HTTP_ACCESS", appenderAccessFile);

  logger->addAppender(ELogLevel::LINFO, "HTTP_DATA_READ", appenderAuditFile);
  logger->addAppender(ELogLevel::LWARN, "HTTP_DATA_READ", appenderAuditFile);
  logger->addAppender(ELogLevel::LERROR, "HTTP_DATA_READ", appenderAuditFile);

  logger->addAppender(ELogLevel::LINFO, "HTTP_CONFIGURATION",
                      appenderAuditFile);
  logger->addAppender(ELogLevel::LWARN, "HTTP_CONFIGURATION",
                      appenderAuditFile);
  logger->addAppender(ELogLevel::LERROR, "HTTP_CONFIGURATION",
                      appenderAuditFile);

  logger->addAppender(ELogLevel::LDEBUG, "HTTP_DATA_READ",
                      Logger::defaultOutAppender);
  logger->addAppender(ELogLevel::LINFO, "HTTP_DATA_READ",
                      Logger::defaultOutAppender);
  logger->addAppender(ELogLevel::LWARN, "HTTP_DATA_READ",
                      Logger::defaultErrAppender);
  logger->addAppender(ELogLevel::LERROR, "HTTP_DATA_READ",
                      Logger::defaultErrAppender);

  logger->addAppender(ELogLevel::LDEBUG, "HTTP_CONFIGURATION",
                      Logger::defaultOutAppender);
  logger->addAppender(ELogLevel::LINFO, "HTTP_CONFIGURATION",
                      Logger::defaultOutAppender);
  logger->addAppender(ELogLevel::LWARN, "HTTP_CONFIGURATION",
                      Logger::defaultErrAppender);
  logger->addAppender(ELogLevel::LERROR, "HTTP_CONFIGURATION",
                      Logger::defaultErrAppender);

  logger->setLevel(ELogLevel::LDEBUG);
}

int32_t main(int argc, char *argv[]) {
  int16_t exitStatus = EXIT_SUCCESS;
  // load logger configuration file to create them appenders
  loadLoggerConfiguration();

  // get singleton logger instance
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  
  logger->info("HTTP_CONFIGURATION", PROJECT_NAME + " - " + APPLICATION_NAME +
                                         " - version " + PROJECT_VERSION);
  // get server configuration
  auto config = ConfigurationServer();

  http::Session::addRequestDispatcher(
      "/api/session",
      [](const boost::beast::http::request<boost::beast::http::string_body>
             &req) {
        HttpTokenEndpoint tokenEndpoint(req);
        tokenEndpoint.dispatchRequest();
        return tokenEndpoint.getResponse();
      });
  http::Session::addRequestDispatcher(
      "/", [](const boost::beast::http::request<boost::beast::http::string_body>
                  &req) {
        LocationEndpoint rootDirectoryEndpoint(req, ".");
        rootDirectoryEndpoint.dispatchRequest();
        return rootDirectoryEndpoint.getResponse();
      });

  logger->info("HTTP_CONFIGURATION", "Enpoints configured");

  auto server =
      http::Server(config.getHostname(), config.getPort(), config.getThreads());

  return exitStatus;
}
