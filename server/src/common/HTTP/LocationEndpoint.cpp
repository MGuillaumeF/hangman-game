#include "LocationEndpoint.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../Logger/Logger.hpp"
#include "./Utils.hpp"
#include "Exception/ParsingException.hpp"

/**
 * @brief Construct a new Location Endpoint object
 *
 * @param req The request to handle
 * @param rootDirectory The start directory of static files hosting
 */
LocationEndpoint::LocationEndpoint(
    const boost::beast::http::request<boost::beast::http::string_body> &req,
    const std::string_view &rootDirectory)
    : http::RestrictiveEndpoint(
          req,
          // Only GET and DELETE method are allowed
          {boost::beast::http::verb::get, boost::beast::http::verb::delete_}),
      m_rootDirectory(rootDirectory) {}

/**
 * @brief Append an HTTP rel-path to a local filesystem path.
 *
 * @param base The base of path to merge
 * @param path The end of path to merge
 * @return std::string  The returned path is normalized for the platform.
 */
std::string LocationEndpoint::pathCat(const boost::beast::string_view &base,
                                      const boost::beast::string_view &path) {
  std::string result(path);
  if (!base.empty()) {
    result = std::string(base);
#ifdef BOOST_MSVC
    // if use MSVC compiler use '\\' as default path separator
    char constexpr path_separator = '\\';
    if (result.back() == path_separator) {
      result.resize(result.size() - 1);
    }
    result.append(path.data(), path.size());
    std::replace(result.begin(), result.end(), '/', path_separator);
#else
    if (char constexpr path_separator = '/'; result.back() == path_separator) {
      result.resize(result.size() - 1);
    }
    result.append(path.data(), path.size());
#endif
  }
  return result;
}

/**
 * @brief Method to read file with GET method
 *
 */
void LocationEndpoint::doGet() {
  // get logger instance
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  // get HTTP request
  const boost::beast::http::request<boost::beast::http::string_body> request =
      this->getRequest();

  // default response is 404 error message when page is not found
  setResponse(http::Utils::not_found(request, request.target()));

  // Request path must be absolute and not contain "..".
  if (!isValidFileTarget(request.target())) {

    setResponse(http::Utils::bad_request(request, "Illegal request-target"));
  } else {
    // Build the path to the requested file
    std::string path = pathCat(m_rootDirectory, request.target());
    if ('/' == request.target().back()) {
      path.append("index.html");
    }

    if (std::filesystem::exists(path)) {
      const std::ifstream stream(path);
      std::stringstream buffer;
      buffer << stream.rdbuf();
      const std::string fileContent = buffer.str();

      // Respond to GET request
      boost::beast::http::response<boost::beast::http::string_body> res =
          http::Utils::wrapper_response(request, boost::beast::http::status::ok,
                                        fileContent,
                                        http::Utils::getMimeType(path));
      // add length of body in meta data of request
      res.content_length(fileContent.size());
      // prepare response body
      res.prepare_payload();
      setResponse(res);
    }
  }
  // trace access log with adapted level
  const std::string accessLog = "[" +
                                std::to_string(getResponse().result_int()) +
                                "] " + std::string(request.target());
  // test if is a success or error
  if (boost::beast::http::to_status_class(getResponse().result_int()) <
      boost::beast::http::status_class::client_error) {
    logger->info("HTTP_ACCESS", accessLog);
  } else {
    logger->error("HTTP_ACCESS", accessLog);
  }
}

/**
 * @brief Method to delete file with DELETE method
 *
 */
void LocationEndpoint::doDelete() {

  // get HTTP request
  const boost::beast::http::request<boost::beast::http::string_body> request =
      this->getRequest();
  // Request path must be absolute and not contain "..".
  if (!isValidFileTarget(request.target())) {
    setResponse(http::Utils::bad_request(request, "Illegal request-target"));
  } else {
    // default response is not found
    setResponse(http::Utils::not_found(request, std::string(request.target())));

    // Request path must be absolute and not contain "..".
    // Build the path to the requested file
    const std::filesystem::path path =
        pathCat(m_rootDirectory, request.target());
    // if file exist remove it
    if (std::filesystem::exists(path)) {
      std::filesystem::remove(path);
      setResponse(
          http::Utils::wrapper_response(request, boost::beast::http::status::ok,
                                        std::string(request.target()), ""));
    }
  }
}

/**
 * @brief Function to test if target of request is a valid path
 *
 * @param boost::beast::string_view The target of request
 * @return true The target of request is valid relative path (desc)
 * @return false The target have bad structure of want to go up on file tree and
 * it's forbidden
 */
bool LocationEndpoint::isValidFileTarget(
    const boost::beast::string_view &target) {
  // a valid target is not empty
  // a valid target start with '/'
  // a valid target haven't '..' in content
  return !(target.empty() || '/' != target[0] ||
           target.find("..") != boost::beast::string_view::npos);
}

