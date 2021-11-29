#include "Session.hpp"
#include "../../api/HttpTokenEndpoint.hpp"
#include "Exception/ParsingException.hpp"
#include "LocationEndpoint.hpp"

const uint8_t MAXIMUM_STREAM_TIME = 30;

namespace http {

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
    const boost::beast::http::request<
        Body, boost::beast::http::basic_fields<Allocator>> &req,
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

  return send(std::move(LocationEndpoint(req, ".").getResponse()));
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
  m_stream.expires_after(std::chrono::seconds(MAXIMUM_STREAM_TIME));

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
    handleRequest(*m_doc_root, m_req, m_lambda);
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

/**
 * @brief methode to add route location for prefix uri
 *
 * @param target The prefix of uri to dispatch requests
 * @param rootDirectory The root directory linked with url
 */
void Session::addLocationDispatcher(const std::string &target,
                                    const std::string &rootDirectory) {
  m_locationDispatcher.try_emplace(target, rootDirectory);
}

} // namespace http
