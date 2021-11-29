#include "LocationEndpoint.hpp"

#include <iostream>

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
    : http::RestrictiveEndpoint(req,
                                // Only POST and DELETE methode are allowed
                                {boost::beast::http::verb::get,
                                 boost::beast::http::verb::post,
                                 boost::beast::http::verb::delete_}) {
  m_rootDirectory = rootDirectory;
}

/**
 * @brief Append an HTTP rel-path to a local filesystem path.
 *
 * @param base The base of path to merge
 * @param path The end of path to merge
 * @return std::string  The returned path is normalized for the platform.
 */
std::string
LocationEndpoint::pathCat(const boost::beast::string_view &base,
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

  // Returns a bad request response
  auto const bad_request = [&request](const boost::beast::string_view &why) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::bad_request, request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(request.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();
    return res;
  };

  // Returns a not found response
  auto const not_found = [&request](const boost::beast::string_view &target) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::not_found, request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(request.keep_alive());
    res.body() = "The resource '" + std::string(target) + "' was not found.";
    res.prepare_payload();
    return res;
  };

  // Returns a server error response
  auto const server_error = [&request](const boost::beast::string_view &what) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::internal_server_error, request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(request.keep_alive());
    res.body() = "An error occurred: '" + std::string(what) + "'";
    res.prepare_payload();
    return res;
  };

  // Request path must be absolute and not contain "..".
  if (request.target().empty() || '/' != request.target()[0] ||
      request.target().find("..") != boost::beast::string_view::npos) {

    setResponse(bad_request("Illegal request-target"));
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

      setResponse(not_found(request.target()));
    } else if (ec) {
      // Handle an unknown error
      setResponse(server_error(ec.message()));

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
      char * const buffer = new char[size];
      body.file().read(buffer, size, ec);

      const std::string fileContent(buffer);
      delete[] buffer;
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
 * @brief Methode to upload file with POST methode
 *
 */
void LocationEndpoint::doPost() {}

/**
 * @brief Methode to replace file with PUT methode
 *
 */
void LocationEndpoint::doPut() {}

/**
 * @brief Methode to delete file with DELETE methode
 *
 */
void LocationEndpoint::doDelete() {}

/**
 * @brief Destroy the Http Location Endpoint:: Http Location Endpoint object
 *
 */
LocationEndpoint::~LocationEndpoint() = default;
