#include "LocationEndpoint.hpp"

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
          // Only GET and DELETE methode are allowed
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
    for (auto &c : result) {
      if (c == '/') {
        c = path_separator;
      }
    }
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
 * @brief Methode to read file with GET methode
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
  if (request.target().empty() || '/' != request.target()[0] ||
      request.target().contains("..")) {

    setResponse(http::Utils::bad_request(request, "Illegal request-target"));
  } else {
    // Build the path to the requested file
    std::string path = pathCat(m_rootDirectory, request.target());
    if ('/' == request.target().back()) {
      path.append("index.html");
    }

    // Attempt to open the file
    boost::beast::error_code ec;
    boost::beast::http::file_body::value_type body;
    body.open(path.c_str(), boost::beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if (ec && ec != boost::beast::errc::no_such_file_or_directory) {
      // Handle an unknown error
      setResponse(http::Utils::server_error(request, ec.message()));

    } else if (!ec) {

      // Cache the size since we need it after the move
      auto const size = body.size();
      // read file with dynamic buffer (vector)
      std::vector<char> buffer;
      buffer.resize(size);
      body.file().read(&buffer[0], size, ec);
      // convert vector to string
      const std::string fileContent(buffer.begin(), buffer.end());

      // Respond to GET request
      boost::beast::http::response<boost::beast::http::string_body> res =
          http::Utils::wrapper_response(
              request, boost::beast::http::status::ok, fileContent,
              std::string(http::Utils::getMimeType(path)));
      res.content_length(size);
      res.prepare_payload();
      setResponse(res);
    }
  }
  // trace access log with adapted level
  const std::string accessLog = "[" +
                                std::to_string(getResponse().result_int()) +
                                "] " + request.target().to_string();
  // test if is a success or error
  if (boost::beast::http::to_status_class(getResponse().result_int()) <
      boost::beast::http::status_class::client_error) {
    logger->info("HTTP_ACCESS", accessLog);
  } else {
    logger->error("HTTP_ACCESS", accessLog);
  }
}

/**
 * @brief Methode to delete file with DELETE methode
 *
 */
void LocationEndpoint::doDelete() {

  // get HTTP request
  const boost::beast::http::request<boost::beast::http::string_body> request =
      this->getRequest();

  // Request path must be absolute and not contain "..".
  if (request.target().empty() || '/' != request.target()[0] ||
      request.target().contains("..")) {

    setResponse(http::Utils::bad_request(request, "Illegal request-target"));
  } else {
    // default response is not found
    setResponse(http::Utils::not_found(request, request.target()));

    // Request path must be absolute and not contain "..".
    if (!(request.target().empty() || '/' != request.target()[0] ||
          request.target().contains("..")) {
      // Build the path to the requested file
      const std::filesystem::path path =
          pathCat(m_rootDirectory, request.target());
      // if file exist remove it
      if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
        setResponse(http::Utils::wrapper_response(
            request, boost::beast::http::status::ok, request.target(), ""));
      }
    }
  }
}

/**
 * @brief Destroy the Http Location Endpoint:: Http Location Endpoint object
 *
 */
LocationEndpoint::~LocationEndpoint() = default;
