#include "./common/Logger/Logger.hpp"

#include "./common/HTTP/Configuration/ConfigurationServer.hpp"
#include "./common/HTTP/HttpServer.hpp"

#include <boost/asio/io_context.hpp>
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

int32_t main(int argc, char *argv[]) {
  int16_t exitStatus = EXIT_SUCCESS;
  g_fs.open("./logs/logfile.log",
            std::fstream::in | std::fstream::out | std::fstream::app);
  g_access_fs.open("./logs/access.log",
                   std::fstream::in | std::fstream::out | std::fstream::app);

  const std::unique_ptr<Logger>& logger = Logger::getInstance();

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

    const std::double_t latitude = pt.get<std::double_t>("point.latitude", 0);

    const std::double_t longitude = pt.get<std::double_t>("point.longitude");

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
    if (4 == m.size()) {
      // example : "data.json(5): garbage after data"
      std::cerr << "Le fichier " << m[1] << " n'est pas un JSON valide, ligne "
                << m[2] << " : " << m[3] << std::endl;
    } else if (3 == m.size()) {
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
  if (5 != argc) {
    logger->error("HTTP_CONFIGURATION",
                  "Usage: HttpServer <address> <port> <doc_root> "
                  "<threads>\nExample:\n    HttpServer 0.0.0.0 8080 . 1\n");
    g_fs.close();
    exitStatus = EXIT_FAILURE;
  } else {
    // If configuration of server is in arguments of execution
    std::vector<std::string> arguments;
    for (uint32_t i = 0; i < argc; i++) {
      arguments.emplace_back(argv[i]);
    }
    // server is started
    auto config = ConfigurationServer(arguments);

    // The io_context is required for all I/O
    boost::asio::io_context ioc{1};

    auto server = HttpServer(ioc, "0.0.0.0", 8080, ".", 1);

    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&threads, &ioc](const boost::system::error_code& ec, const int32_t& n){
      Logger::getInstance()->info("HTTP_CONFIGURATION", std::string("IO Context stop with ") + ec.message() + std::string(" and handler code : ") + std::to_string(n));
      for (auto i = 0; i < threads; i++) {
        ioc.stop();
      }
    });
    // run server listeners on context
    ioc.run();
    Logger::getInstance()->info("HTTP_CONFIGURATION", "---------------- HERE 1 ----------------");

    g_fs.close();
  }
  logger->info("HTTP_CONFIGURATION", "---------------- HERE 2 ----------------");
  return exitStatus;
}
