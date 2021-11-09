#ifndef __HTTP_SESSION_H__
#define __HTTP_SESSION_H__

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "HttpUtils.hpp"

using requestHandler_t =
    boost::beast::http::response<boost::beast::http::string_body> (*)(
        const boost::beast::http::request<boost::beast::http::string_body> &);
/**
 * Handles an HTTP server connection
 */
class HttpSession : public std::enable_shared_from_this<HttpSession> {
  /**
   * @brief This is the C++11 equivalent of a generic lambda.
   * The function object is used to send an HTTP message.
   *
   */
  struct send_lambda {
    HttpSession &self_;
    /**
     * @brief Construct a new send lambda object
     *
     * @param self pointer on struct
     */
    explicit send_lambda(HttpSession &self) : self_(self) {}

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
              &HttpSession::onWrite, self_.shared_from_this(), sp->need_eof()));
    }
  };

  boost::beast::tcp_stream m_stream;
  boost::beast::flat_buffer m_buffer;
  std::shared_ptr<std::string const> m_doc_root;
  boost::beast::http::request<boost::beast::http::string_body> m_req;
  std::shared_ptr<void> m_res;
  send_lambda m_lambda;

  std::map<std::string, requestHandler_t, std::less<>> m_requestDispatcher;

  /**
   * @brief Append an HTTP rel-path to a local filesystem path.
   *
   * @param base The base of path to merge
   * @param path The end of path to merge
   * @return std::string  The returned path is normalized for the platform.
   */
  std::string pathCat(const boost::beast::string_view& base,
                      const boost::beast::string_view& path) const;

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
  void
  handleRequest(const boost::beast::string_view& doc_root,
                boost::beast::http::request<
                    Body, boost::beast::http::basic_fields<Allocator>> &&req,
                Send &&send);

public:
  /**
   * @brief Construct a new Http Session object
   *
   * @param socket Take ownership of the socket's stream
   * @param doc_root The path of static files
   */
  HttpSession(boost::asio::ip::tcp::socket &&socket,
              std::shared_ptr<std::string const> const &doc_root)
      : m_stream(std::move(socket)), m_doc_root(doc_root), m_lambda(*this) {}

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
  void onRead(const boost::beast::error_code& ec, const std::size_t& bytes_transferred);
  /**
   * @brief Method to write response
   *
   * @param close The decision to close socket or not
   * @param ec The error code of previous step
   * @param bytes_transferred The size of bytes transferred
   */
  void onWrite(const bool& close, const boost::beast::error_code& ec,
               const std::size_t& bytes_transferred);
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
};

#endif
