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
 * @brief Construct a new Http Location Endpoint:: Http Location Endpoint object
 *
 * @param req The request to handle
 */
LocationEndpoint::LocationEndpoint(
    const boost::beast::http::request<boost::beast::http::string_body> &req,
    const std::string &rootDirectory)
    : http::RestrictiveEndpoint(
          req,
          // Only POST and DELETE methode are allowed
          {boost::beast::http::verb::get, boost::beast::http::verb::delete_}) {
  m_rootDirectory = rootDirectory;
}

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
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  // get HTTP request
  const boost::beast::http::request<boost::beast::http::string_body> request =
      this->getRequest();

  // Request path must be absolute and not contain "..".
  if (request.target().empty() || '/' != request.target()[0] ||
      request.target().find("..") != boost::beast::string_view::npos) {

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
    if (ec == boost::beast::errc::no_such_file_or_directory) {

      setResponse(http::Utils::not_found(request, request.target()));
    } else if (ec) {
      // Handle an unknown error
      setResponse(http::Utils::server_error(request, ec.message()));

    } else {

      // Cache the size since we need it after the move
      auto const size = body.size();

      // Respond to GET request
      boost::beast::http::response<boost::beast::http::string_body> res{
          boost::beast::http::status::ok, request.version()};
      res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set(boost::beast::http::field::content_type,
              http::Utils::getMimeType(path));
      res.content_length(size);
      res.keep_alive(request.keep_alive());
      std::vector<char> buffer;
      buffer.resize(size);
      body.file().read(&buffer[0], size, ec);
      const std::string fileContent(buffer.begin(), buffer.end());
      res.body() = fileContent;
      res.prepare_payload();
      setResponse(res);

      // trace access log with adapted level
      const std::string accessLog = "[" + std::to_string(res.result_int()) +
                                    "] " + request.target().to_string();
      if (boost::beast::http::to_status_class(res.result_int()) <
          boost::beast::http::status_class::client_error) {
        logger->info("HTTP_ACCESS", accessLog);
      } else {
        logger->error("HTTP_ACCESS", accessLog);
      }

      setResponse(res);
    }
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
  // default response is not found
  setResponse(http::Utils::not_found(request, request.target()));

  // Request path must be absolute and not contain "..".
  if (!(request.target().empty() || '/' != request.target()[0] ||
        request.target().find("..") != boost::beast::string_view::npos)) {
    // Build the path to the requested file
    const std::string path = pathCat(m_rootDirectory, request.target());
    if (std::filesystem::exists(path)) {
      std::filesystem::remove(path);
      setResponse(http::Utils::wrapper_response(
          request, boost::beast::http::status::ok, request.target(), ""));
    }
  }
}

/**
 * @brief Destroy the Http Location Endpoint:: Http Location Endpoint object
 *
 */
LocationEndpoint::~LocationEndpoint() = default;
