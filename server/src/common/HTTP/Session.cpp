#include "Session.hpp"
#include "../../api/HttpTokenEndpoint.hpp"
#include "Exception/ParsingException.hpp"
#include "LocationEndpoint.hpp"
#include <iostream>
const uint8_t MAXIMUM_STREAM_TIME = 30;

namespace http {

// request dispatcher storage
std::list<std::pair<std::string, requestHandler_t>>
    Session::m_requestDispatcher = {};

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
 * @param req The HTTP request
 * @param send The Sender to emit HTTP response
 */
void Session::handleRequest(
    const boost::beast::http::request<
        Body, boost::beast::http::basic_fields<Allocator>> &req,
    Send &&send) const {

  const std::unique_ptr<Logger> &logger = Logger::getInstance();

  logger->info("HTTP_DATA_READ",
               "request received on target " + std::string(req.target()));

  bool responseDefined = false;

  try {
    for (const auto &[endpointEntry, endpointHandler] : m_requestDispatcher) {
      if ((0 == req.target().compare(endpointEntry)) ||
          req.target().starts_with(endpointEntry)) {
        logger->info("HTTP_ACCESS", "handleRequest - " + endpointEntry);
        send(std::move(endpointHandler(req)));
        responseDefined = true;
        break;
      }
    }
  } catch (const ParsingException &ex) {
    logger->error("HTTP_DATA_READ",
                  "handleRequest - parsing error : " + std::string(ex.what()));

    send(Utils::bad_request(req, "parsing error"));
    responseDefined = true;
  } catch (const std::exception &ex) {
    logger->error("HTTP_DATA_READ",
                  "handleRequest - parsing error : " + std::string(ex.what()));

    send(Utils::server_error(req, "api call error"));
    responseDefined = true;
  }
  if (!responseDefined) {
    send(std::move(Utils::not_found(req, req.target())));
  }
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
    handleRequest(m_req, m_lambda);
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
 * @brief method to add route for prefix uri
 *
 * @param target The prefix of uri to dispatch requests
 * @param handler The handler function to call
 */
void Session::addRequestDispatcher(const std::string &target,
                                   const requestHandler_t &handler) {
  m_requestDispatcher.emplace_back(target, handler);
}

} // namespace http
