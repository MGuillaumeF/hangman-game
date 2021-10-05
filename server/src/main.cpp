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
// #include <boost/property_tree/ptree.hpp>
// #include <boost/property_tree/xml_parser.hpp>

// struct Server {
//   std::string description;
//   std::string hostname;
//   std::string path;
//   unsigned short thread;
//   unsigned port;
// };

// typedef std::vector<Server> Servers;

std::fstream g_fs;
/**
 * To write a log message
 * @param msg The message to print
 */
void appenderFile(const std::string &message) { g_fs << message << std::endl; }

int main(int argc, char *argv[]) {
  g_fs.open("./logs/logfile.log",
            std::fstream::in | std::fstream::out | std::fstream::app);

  Logger *logger = Logger::getInstance();
  logger->addAppender(Logger::LINFO, "HTTP_DATA_READ", appenderFile);
  logger->addAppender(Logger::LINFO, "HTTP_CONFIGURATION", appenderFile);
  logger->addAppender(Logger::LWARN, "HTTP_DATA_READ", appenderFile);
  logger->addAppender(Logger::LWARN, "HTTP_CONFIGURATION", appenderFile);
  logger->addAppender(Logger::LERROR, "HTTP_DATA_READ", appenderFile);
  logger->addAppender(Logger::LERROR, "HTTP_CONFIGURATION", appenderFile);

  logger->addAppender(Logger::LDEBUG, "HTTP_DATA_READ",
                      Logger::defaultOutAppender);
  logger->addAppender(Logger::LINFO, "HTTP_DATA_READ",
                      Logger::defaultOutAppender);
  logger->addAppender(Logger::LINFO, "HTTP_CONFIGURATION",
                      Logger::defaultOutAppender);
  logger->addAppender(Logger::LERROR, "HTTP_DATA_READ",
                      Logger::defaultErrAppender);
  logger->addAppender(Logger::LERROR, "HTTP_CONFIGURATION",
                      Logger::defaultErrAppender);

  std::vector<std::string> msg{"Hello", "C++",     "World",
                               "from",  "VS Code", "and the C++ extension!"};

  for (const std::string &word : msg) {
    std::cout << word << " ";
  }
  std::cout << std::endl;

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

  // boost::property_tree::ptree pt;
  // boost::property_tree::read_xml("../resources/configuration.xml", pt);

  // Servers confServers;
  // Server s;

  // boost::property_tree::ptree servers = pt.get_child("servers");
  // for (const boost::property_tree::ptree::value_type &kv : servers) {
  //   s.description = kv.second.get<std::string>("description");
  //   std::cerr << "description  : " << s.description << std::endl;
  // }
  // s.description = pt.get<std::string>("server.description");
  // std::cerr << "description  : " << s.description;
  // confServers.push_back(s);

  // If configuration of server is in arguments of execution
  // server is started
  ConfigurationServer config = ConfigurationServer(argv);
  HttpServer server = HttpServer("0.0.0.0", 8080, ".",
                                 3); //(argv[1], argv[2], argv[3], argv[4]);

  g_fs.close();
  return EXIT_SUCCESS;
}