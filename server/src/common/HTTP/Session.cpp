
#include "Session.hpp"
#include "../../api/HttpTokenEndpoint.hpp"

namespace http {

/**
 * @brief Append an HTTP rel-path to a local filesystem path.
 *
 * @param base The base of path to merge
 * @param path The end of path to merge
 * @return std::string  The returned path is normalized for the platform.
 */
std::string Session::pathCat(const boost::beast::string_view &base,
                             const boost::beast::string_view &path) const {
  std::string result(path);
  if (!base.empty()) {
    result = std::string(base);
#ifdef BOOST_MSVC
    if (char constexpr path_separator = '\\'; result.back() == path_separator) {
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

/** This function produces an HTTP response for the given
 * request. The type of the response object depends on the
 * contents of the request, so the interface requires the
 * caller to pass a generic lambda for receiving the response.
 */
template <class Body, class Allocator, class Send>
/**
 * @brief This function produces an HTTP response for the given
 * request. The type of the response object depends on the
 * contents of the request, so the interface requires the
 * caller to pass a generic lambda for receiving the response.
 *
 * @param doc_root The path of static files of file server
 * @param req The HTTP request
 * @param send The Sender to emit HTTP response
 */
void Session::handleRequest(
    const boost::beast::string_view &doc_root,
    boost::beast::http::request<
        Body, const boost::beast::http::basic_fields<Allocator>> &req,
    Send &&send) {

  const std::unique_ptr<Logger> &logger = Logger::getInstance();

  logger->info("HTTP_DATA_READ",
               "request received on target " + req.target().to_string());

  // Returns a bad request response
  auto const bad_request = [&req](const boost::beast::string_view &why) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::bad_request, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();
    return res;
  };

  // Returns a not found response
  auto const not_found = [&req](const boost::beast::string_view &target) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::not_found, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "The resource '" + std::string(target) + "' was not found.";
    res.prepare_payload();
    return res;
  };

  // Returns a server error response
  auto const server_error = [&req](const boost::beast::string_view &what) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::internal_server_error, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "An error occurred: '" + std::string(what) + "'";
    res.prepare_payload();
    return res;
  };

  try {
    if (0 == req.target().compare("/api/token")) {
      logger->info("HTTP_ACCESS", "handleRequest - /api/token");
      HttpTokenEndpoint tokenEndpoint(req);
      tokenEndpoint.dispatchRequest();
      boost::beast::http::response<boost::beast::http::string_body> response =
          tokenEndpoint.getResponse();
      return send(std::move(response));
    }

  } catch (const ParsingException &ex) {
    logger->error("HTTP_DATA_READ",
                  "handleRequest - parsing error : " + std::string(ex.what()));

    return send(bad_request("parsing error"));
  }
  // Make sure we can handle the method
  if (req.method() != boost::beast::http::verb::get &&
      req.method() != boost::beast::http::verb::head)
    return send(bad_request("Unknown HTTP-method"));

  // Request path must be absolute and not contain "..".
  if (req.target().empty() || '/' != req.target()[0] ||
      req.target().find("..") != boost::beast::string_view::npos)
    return send(bad_request("Illegal request-target"));

  // Build the path to the requested file
  std::string path = pathCat(doc_root, req.target());
  if ('/' == req.target().back())
    path.append("index.html");

  // Attempt to open the file
  boost::beast::error_code ec;
  boost::beast::http::file_body::value_type body;
  body.open(path.c_str(), boost::beast::file_mode::scan, ec);

  // Handle the case where the file doesn't exist
  if (ec == boost::beast::errc::no_such_file_or_directory)
    return send(not_found(req.target()));

  // Handle an unknown error
  if (ec)
    return send(server_error(ec.message()));

  // Cache the size since we need it after the move
  auto const size = body.size();

  // Respond to HEAD request
  if (req.method() == boost::beast::http::verb::head) {
    boost::beast::http::response<boost::beast::http::empty_body> res{
        boost::beast::http::status::ok, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, Utils::getMimeType(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
  }

  // Respond to GET request
  boost::beast::http::response<boost::beast::http::file_body> res{
      std::piecewise_construct, std::make_tuple(std::move(body)),
      std::make_tuple(boost::beast::http::status::ok, req.version())};
  res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(boost::beast::http::field::content_type, Utils::getMimeType(path));
  res.content_length(size);
  res.keep_alive(req.keep_alive());
  const std::string accessLog =
      "[" + std::to_string(res.result_int()) + "] " + req.target().to_string();
  if (res.result_int() < 400) {
    logger->info("HTTP_ACCESS", accessLog);
  } else {
    logger->error("HTTP_ACCESS", accessLog);
  }
  return send(std::move(res));
}

/**
 * @brief Start the asynchronous operation
 *
 */
void Session::run() {
  // We need to be executing within a strand to perform async operations
  // on the I/O objects in this session. Although not strictly necessary
  // for single-threaded contexts, this example code is written to be
  // thread-safe by default.
  boost::asio::dispatch(
      m_stream.get_executor(),
      boost::beast::bind_front_handler(&Session::doRead, shared_from_this()));
}

/**
 * @brief Method to start async reading of request
 *
 */
void Session::doRead() {
  // Make the request empty before reading,
  // otherwise the operation behavior is undefined.
  m_req = {};

  // Set the timeout.
  m_stream.expires_after(std::chrono::seconds(30));

  // Read a request
  boost::beast::http::async_read(
      m_stream, m_buffer, m_req,
      boost::beast::bind_front_handler(&Session::onRead, shared_from_this()));
}

/**
 * @brief Method to read request
 *
 * @param ec The error code of previous step
 * @param bytes_transferred The size of bytes transferred
 */
void Session::onRead(const boost::beast::error_code &ec,
                     const std::size_t &bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // This means they closed the connection
  if (ec == boost::beast::http::error::end_of_stream) {
    doClose();
  } else if (ec) {
    Logger::getInstance()->error("HTTP_CONFIGURATION",
                                 " On read request error : " + ec.message());
  } else {
    // Send the response
    handleRequest(*m_doc_root, std::move(m_req), m_lambda);
  }
}

/**
 * @brief Method to write response
 *
 * @param close The decision to close socket or not
 * @param ec The error code of previous step
 * @param bytes_transferred The size of bytes transferred
 */
void Session::onWrite(const bool &close, const boost::beast::error_code &ec,
                      const std::size_t &bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    Logger::getInstance()->error("HTTP_CONFIGURATION",
                                 " On write request error : " + ec.message());
  } else if (close) {
    // This means we should close the connection, usually because
    // the response indicated the "Connection: close" semantic.
    doClose();
  } else {
    // We're done with the response so delete it
    m_res = nullptr;

    // Read another request
    doRead();
  }
}

/**
 * @brief method to close TCP/IP socket
 *
 */
void Session::doClose() {
  // Send a TCP shutdown
  boost::beast::error_code ec;
  m_stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);

  // At this point the connection is closed gracefully
}

/**
 * @brief methode to add route for prefix uri
 *
 * @param target The prefix of uri to dispatch requests
 * @param handler The handler function to call
 */
void Session::addRequestDispatcher(const std::string &target,
                                   const requestHandler_t &handler) {
  m_requestDispatcher.try_emplace(target, handler);
}

} // namespace HTTP
