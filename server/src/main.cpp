#include "./common/Logger/Logger.hpp"

#include "./common/HTTP/Configuration/ConfigurationServer.hpp"
#include "./common/HTTP/HttpServer.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

#include <boost/lambda/lambda.hpp>
#include <boost/property_tree/json_parser.hpp>

// global streams for files loggers
std::fstream g_fs;
// light http access logs
std::fstream g_access_fs;

/**
 * To write a log message
 * @param message The message to print
 */
void appenderFile(const std::string &message) { g_fs << message << std::endl; }
/**
 * To write a log message
 * @param message The message to print
 */
void appenderAccessFile(const std::string &message) {
  g_access_fs << message << std::endl;
}

int main(int argc, char *argv[]) {
  g_fs.open("./logs/logfile.log",
            std::fstream::in | std::fstream::out | std::fstream::app);
  g_access_fs.open("./logs/access.log",
                   std::fstream::in | std::fstream::out | std::fstream::app);

  Logger *logger = Logger::getInstance();

  logger->addAppender(ELogLevel::LDEBUG, "HTTP_ACCESS",
                      Logger::defaultOutAppender);
  logger->addAppender(ELogLevel::LINFO, "HTTP_ACCESS", appenderAccessFile);
  logger->addAppender(ELogLevel::LWARN, "HTTP_ACCESS", appenderAccessFile);
  logger->addAppender(ELogLevel::LERROR, "HTTP_ACCESS", appenderAccessFile);

  logger->addAppender(ELogLevel::LINFO, "HTTP_DATA_READ", appenderFile);
  logger->addAppender(ELogLevel::LWARN, "HTTP_DATA_READ", appenderFile);
  logger->addAppender(ELogLevel::LERROR, "HTTP_DATA_READ", appenderFile);

  logger->addAppender(ELogLevel::LINFO, "HTTP_CONFIGURATION", appenderFile);
  logger->addAppender(ELogLevel::LWARN, "HTTP_CONFIGURATION", appenderFile);
  logger->addAppender(ELogLevel::LERROR, "HTTP_CONFIGURATION", appenderFile);

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

  boost::property_tree::ptree pt;
  try {

    boost::property_tree::read_json("../resources/data.json", pt);

    const long double latitude = pt.get<long double>("point.latitude", 0);

    const long double longitude = pt.get<long double>("point.longitude");

    std::cout << "Le point est : " << std::endl
              << "Latitude : " << latitude << std::endl
              << "Longitude : " << longitude << std::endl;
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
    if (m.size() == 4) {
      // example : "data.json(5): garbage after data"
      std::cerr << "Le fichier " << m[1] << " n'est pas un JSON valide, ligne "
                << m[2] << " : " << m[3] << std::endl;
    } else if (m.size() == 3) {
      // example : data.json: cannot open file
      std::cerr << "Le fichier " << m[1]
                << " n'est pas un JSON valide : " << m[2] << std::endl;
    } else {
      std::cerr << "Erreur de parsing du fichier " << ex.what() << std::endl;
    }
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
    // ERROR TYPE : ENOKEY, mandatory key not found
    // FIELD : [NAME, ERROR]
    std::cerr << "data.json ne contient pas un attribut obligatoire : "
              << ex.what() << std::endl;
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_data> &ex) {
    // ERROR TYPE : EVALUETYPE, key found with bad value type
    // FIELD : [NAME, ERROR]
    std::cerr << "data.json contient une données de type invalide : "
              << ex.what() << std::endl;
  }

  // Check command line arguments.
  if (argc != 5) {
    logger->error("HTTP_CONFIGURATION",
                  "Usage: HttpServer <address> <port> <doc_root> "
                  "<threads>\nExample:\n    HttpServer 0.0.0.0 8080 . 1\n");
    g_fs.close();
    return EXIT_FAILURE;
  }

  // If configuration of server is in arguments of execution
  // server is started
  auto config = ConfigurationServer(argv);
  auto server = HttpServer("0.0.0.0", 8080, ".", 3);

  g_fs.close();
  return EXIT_SUCCESS;
}
