#include "Utils.hpp"
#include "../Logger/Logger.hpp"
#include <boost/beast/version.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "./Exception/ParsingException.hpp"

namespace http {
/**
 * @brief configuration map of mimetype where key is extension, populated at the
 * first mime-type search
 */
std::map<std::string, std::string, std::less<>> Utils::s_extTomimtype = {};

/**
 * @brief The default constructor of Utils class
 */
Utils::Utils() = delete;

/**
 * @brief Return a reasonable mime type based on the extension of a file.
 * @param path the path of request
 * @return the mime-type of file
 */
std::string Utils::getMimeType(const std::string &path) {
  const std::string ext = path.find('.') != std::string::npos ? path.substr(path.rfind('.')) : "";

  // set default mime type
  std::string l_sMimeType = "application/text";
  // get logger for trace
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  // if mime type configuration map is empty log configuration file
  if (Utils::s_extTomimtype.empty()) {
    Utils::loadMimTypesConfiguration();
  }
  // get the mime type from configuration if ext is found in loaded
  // configuration
  if (Utils::s_extTomimtype.contains(ext)) {
    l_sMimeType = Utils::s_extTomimtype.at(ext);
  } else {
    logger->warn("HTTP_CONFIGURATION",
                 "MimeType of File not found for extension : " +
                     ext);
  }
  logger->debug("HTTP_CONFIGURATION", "MimeType of File used : " + l_sMimeType);
  return l_sMimeType;
}

/**
 * @brief Load Mim Types Configuration map
 */
void Utils::loadMimTypesConfiguration() {
  // get logger for trace
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  logger->debug("HTTP_CONFIGURATION",
                "MimeType mapper is empty, readding configuration file");
  boost::property_tree::ptree mimeTypesConfig;
  // parse configuration file as xml representation of preperty tree
  boost::property_tree::read_xml("./configuration/mime-types.xml",
                                 mimeTypesConfig);
  // for each mime-type row get extension and associated mime type value
  for (const auto &[_arrayKey, mimeType] :
       mimeTypesConfig.get_child("mime-types")) {
    const std::string l_ext = mimeType.get<std::string>("<xmlattr>.ext");
    const std::string l_value = mimeType.get<std::string>("<xmlattr>.value");
    logger->debug("HTTP_CONFIGURATION",
                  "MimeType pair found [" + l_ext + ", " + l_value + "]");
    // insert value in configuration map
    s_extTomimtype.try_emplace(l_ext, l_value);
  }
  logger->debug("HTTP_CONFIGURATION",
                "MimeType mapper loaded for " +
                    std::to_string(Utils::s_extTomimtype.size()) +
                    " extensions");
}

/**
 * @brief Get the Tree object of Body request
 *
 * @param req The request of client
 * @return boost::property_tree::ptree The property tree of body content
 */
boost::property_tree::ptree Utils::getBodyTree(
    const boost::beast::http::request<boost::beast::http::string_body> &req) {
  // get logger for trace
  const std::unique_ptr<Logger> &logger = Logger::getInstance();

  std::stringstream l_stream(req.body());
  boost::property_tree::ptree requestBodyTree;

  if (req.find(boost::beast::http::field::content_type) != req.end()) {

    // if content-type is JSON
    const boost::string_view contentType =
        req.at(boost::beast::http::field::content_type);
    if (0 == contentType.compare("application/json")) {
      logger->debug("HTTP_DATA_READ",
                    "getBodyTree - json body content expected");
      try {
        // read JSON request body in string stream
        boost::property_tree::read_json(l_stream, requestBodyTree);
      } catch (const std::exception &ex) {
        // request body tree is invalid
        logger->error("HTTP_DATA_READ",
                      "getBodyTree - JSON body has invalid structure" +
                          std::string(ex.what()));
        throw ParsingException("body has invalid structure");
      }
      // parse as XML body
    } else if (0 == contentType.compare("application/xml")) {
      logger->debug("HTTP_DATA_READ",
                    "getBodyTree - xml body content expected");
      try {
        // read XML request body in string stream
        boost::property_tree::read_xml(l_stream, requestBodyTree);
      } catch (const std::exception &ex) {
        // if xml nota valid throw parse error
        logger->error("HTTP_DATA_READ",
                      "getBodyTree - XML body has invalid structure" +
                          std::string(ex.what()));
        throw ParsingException("body has invalid structure");
      }
    } else {
      // content type header value is unknown
      logger->error("HTTP_DATA_READ",
                    "getBodyTree - content type not supported");
      throw ParsingException("content type is not valid");
    }
  } else {
    // content type header not found
    logger->error("HTTP_DATA_READ",
                  "getBodyTree - content type header not found");
    throw ParsingException("content type is not present");
  }
  return requestBodyTree;
}

/**
 * @brief static method of default bad_request response
 *
 * @param req The request of client
 * @param why The reason of bad request
 * @return boost::beast::http::response<boost::beast::http::string_body> The
 * body response
 */
boost::beast::http::response<boost::beast::http::string_body>
Utils::bad_request(
    const boost::beast::http::request<boost::beast::http::string_body> &req,
    const std::string_view &why) {
  return wrapper_response(req, boost::beast::http::status::bad_request, why);
}

/**
 * @brief static method of default not_found response
 *
 * @param req The request of client
 * @param target The target not found
 * @return boost::beast::http::response<boost::beast::http::string_body> The
 * body response
 */
boost::beast::http::response<boost::beast::http::string_body> Utils::not_found(
    const boost::beast::http::request<boost::beast::http::string_body> &req,
    const boost::beast::string_view &target) {
  return wrapper_response(req, boost::beast::http::status::not_found,
                          "The resource '" + std::string(target) +
                              "' was not found.");
}

/**
 * @brief static method of default server_error response
 *
 * @param req The request of client
 * @param what The server error message
 * @return boost::beast::http::response<boost::beast::http::string_body> The
 * body response
 */
boost::beast::http::response<boost::beast::http::string_body>
Utils::server_error(
    const boost::beast::http::request<boost::beast::http::string_body> &req,
    const boost::beast::string_view &what) {
  return wrapper_response(req,
                          boost::beast::http::status::internal_server_error,
                          "An error occurred: '" + std::string(what) + "'");
}

/**
 * @brief The response writer wrapper
 *
 * @param req The request of client
 * @param status The status of response HTTP
 * @param body The body of response
 * @param contentType The content type of body response
 * @return boost::beast::http::response<boost::beast::http::string_body>
 */
boost::beast::http::response<boost::beast::http::string_body>
Utils::wrapper_response(
    const boost::beast::http::request<boost::beast::http::string_body> &req,
    const boost::beast::http::status &status,
    const std::string_view &body,
    const std::string_view &contentType) {
  boost::beast::http::response<boost::beast::http::string_body> res{
      status, req.version()};
  res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(boost::beast::http::field::content_type, std::string(contentType));
  res.keep_alive(req.keep_alive());
  res.body() = std::string(body);
  res.prepare_payload();
  return res;
}

} // namespace http
