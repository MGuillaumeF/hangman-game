#ifndef __HTTP_SESSION_HPP__
#define __HTTP_SESSION_HPP__

#include <functional>

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Utils.hpp"

namespace http {

using requestHandler_t =
    boost::beast::http::response<boost::beast::http::string_body> (*)(
        const boost::beast::http::request<boost::beast::http::string_body> &);
/**
 * Handles an HTTP server connection
 */
class Session : public std::enable_shared_from_this<Session> {
  /**
   * @brief This is the C++11 equivalent of a generic lambda.
   * The function object is used to send an HTTP message.
   *
   */
  struct send_lambda {
    Session &self_;
    /**
     * @brief Construct a new send lambda object
     *
     * @param self pointer on struct
     */
    explicit send_lambda(Session &self) : self_(self) {}

    template <bool isRequest, class Body, class Fields>
    /**
     * @brief operator parenthesis to manage handled request
     *
     * @param msg
     */
    void operator()(
        boost::beast::http::message<isRequest, Body, Fields> &&msg) const {
      // The lifetime of the message has to extend
      // for the duration of the async operation so
      // we use a shared_ptr to manage it.
      auto sp = std::make_shared<
          boost::beast::http::message<isRequest, Body, Fields>>(std::move(msg));

      // Store a type-erased version of the shared
      // pointer in the class to keep it alive.
      self_.m_res = sp;

      // Write the response
      boost::beast::http::async_write(
          self_.m_stream, *sp,
          boost::beast::bind_front_handler(
              &Session::onWrite, self_.shared_from_this(), sp->need_eof()));
    }
  };

  boost::beast::tcp_stream m_stream;
  boost::beast::flat_buffer m_buffer;
  boost::beast::http::request<boost::beast::http::string_body> m_req;
  std::shared_ptr<void> m_res;
  send_lambda m_lambda;

  std::map<std::string, requestHandler_t, std::less<>> m_requestDispatcher;
  std::map<std::string, std::string, std::less<>> m_locationDispatcher;

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
  void
  handleRequest(const boost::beast::http::request<
                    Body, boost::beast::http::basic_fields<Allocator>> &req,
                Send &&send);

public:
  /**
   * @brief Construct a new Http Session object
   *
   * @param socket Take ownership of the socket's stream
   */
  explicit Session(boost::asio::ip::tcp::socket &&socket)
      : m_stream(std::move(socket)), m_lambda(*this) {}

  /**
   * @brief Start the asynchronous operation
   *
   */
  void run();
  /**
   * @brief Method to start async reading of request
   *
   */
  void doRead();
  /**
   * @brief Method to read request
   *
   * @param ec The error code of previous step
   * @param bytes_transferred The size of bytes transferred
   */
  void onRead(const boost::beast::error_code &ec,
              const std::size_t &bytes_transferred);
  /**
   * @brief Method to write response
   *
   * @param close The decision to close socket or not
   * @param ec The error code of previous step
   * @param bytes_transferred The size of bytes transferred
   */
  void onWrite(const bool &close, const boost::beast::error_code &ec,
               const std::size_t &bytes_transferred);
  /**
   * @brief method to close TCP/IP socket
   *
   */
  void doClose();
  /**
   * @brief methode to add route for prefix uri
   *
   * @param target The prefix of uri to dispatch requests
   * @param handler The handler function to call
   */
  void addRequestDispatcher(const std::string &target,
                            const requestHandler_t &handler);

  /**
   * @brief methode to add route location for prefix uri
   *
   * @param target The prefix of uri to dispatch requests
   * @param rootDirectory The root directory linked with url
   */
  void addLocationDispatcher(const std::string &target,
                             const std::string &rootDirectory);
};

} // namespace http
#endif
