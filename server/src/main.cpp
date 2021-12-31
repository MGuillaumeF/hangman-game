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

  loadLoggerConfiguration();

  const std::unique_ptr<Logger> &logger = Logger::getInstance();

  boost::property_tree::ptree pt;
  try {

    boost::property_tree::read_json("../resources/data.json", pt);

    const std::double_t latitude = pt.get<std::double_t>("point.latitude", 0);

    const std::double_t longitude = pt.get<std::double_t>("point.longitude");

    logger->info("HTTP_CONFIGURATION",
                 "Le point est : \r\n Latitude : " + std::to_string(latitude) +
                     "\r\n Longitude : " + std::to_string(longitude));
  } catch (const boost::wrapexcept<
           boost::property_tree::json_parser::json_parser_error> &ex) {
    // ERROR TYPE : ESTRUCTOBJ, object has bad JSON structure
    // ERROR TYPE : EREADOBJ, IO
    // ERROR TYPE : EUNKOBJ unknow error on object
    std::smatch m;
    const std::string errorStr(ex.what());
    // filename, number line error, error message
    std::regex re("(.+)\\((\\d+)\\): (.*)");
    std::regex_search(errorStr, m, re);
    if (!m.size()) {
      re = std::regex("(.+): (.*)");
      std::regex_search(errorStr, m, re);
    }
    const uint8_t jsonStructErrorSize = 4;
    const uint8_t accessFileErrorSize = 3;
    if (jsonStructErrorSize == m.size()) {
      // example : "data.json(5): garbage after data"
      const uint8_t filenameIndex = 1;
      const uint8_t lineIndex = 2;
      const uint8_t messageIndex = 3;
      logger->error("HTTP_CONFIGURATION",
                    "Le fichier " + m[filenameIndex].str() +
                        " n'est pas un JSON valide, ligne " +
                        m[lineIndex].str() + " : " + m[messageIndex].str());
    } else if (accessFileErrorSize == m.size()) {
      // example : data.json: cannot open file
      const uint8_t filenameIndex = 1;
      const uint8_t messageIndex = 2;
      logger->error("HTTP_CONFIGURATION",
                    "Le fichier " + m[filenameIndex].str() +
                        " n'est pas un JSON valide : " + m[messageIndex].str());
    } else {
      logger->error("HTTP_CONFIGURATION",
                    "Erreur de parsing du fichier " + std::string(ex.what()));
    }
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
    // ERROR TYPE : ENOKEY, mandatory key not found
    // FIELD : [NAME, ERROR]
    logger->error("HTTP_CONFIGURATION",
                  "data.json ne contient pas un attribut obligatoire : " +
                      std::string(ex.what()));
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_data> &ex) {
    // ERROR TYPE : EVALUETYPE, key found with bad value type
    // FIELD : [NAME, ERROR]
    logger->error("HTTP_CONFIGURATION",
                  "data.json contient une données de type invalide : " +
                      std::string(ex.what()));
  }

  // get server configuration
  auto config = ConfigurationServer();

  http::Session::addRequestDispatcher(
      "/api/token",
      [](const boost::beast::http::request<boost::beast::http::string_body>
             &req) {
        HttpTokenEndpoint tokenEndpoint(req);
        tokenEndpoint.dispatchRequest();
        return tokenEndpoint.getResponse();
      });
  http::Session::addRequestDispatcher(
      config.getRoot(), [](const boost::beast::http::request<boost::beast::http::string_body>
                  &req) {
        LocationEndpoint rootDirectoryEndpoint(req, ".");
        rootDirectoryEndpoint.dispatchRequest();
        return rootDirectoryEndpoint.getResponse();
      });
  auto server = http::Server(config.getHostname(), config.getPort(), config.getThreads());

  return exitStatus;
}
