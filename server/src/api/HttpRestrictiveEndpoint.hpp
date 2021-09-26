#ifndef __HTTP_RESTRICTIVE_ENDPOINT_HPP__
#define __HTTP_RESTRICTIVE_ENDPOINT_HPP__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <map>

/**
 * Http Endpoint Class ito create HTTP response
 */
template <class Body, class Allocator, class Send>
class HttpRestrictiveEndpoint {
private:
  void doGet() { writeNotImplementedResponse(); }

  void doPost() { writeNotImplementedResponse(); }

  void doPut() { writeNotImplementedResponse(); }

  void doDelete() { writeNotImplementedResponse(); }

  void doPatch() { writeNotImplementedResponse(); }

protected:
  boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>
      m_request;

  Send m_send;
  std::map<boost::beast::http::verb, bool> m_allowedMethods;

  void writeMethodNotAllowed() {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::method_not_allowed, m_request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = "The method is not allowed.";
    res.prepare_payload();
    m_send(res);
  }

  void writeNotImplementedResponse() {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::not_implemented, m_request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = "The method is not implemented.";
    res.prepare_payload();
    m_send(res);
  }

  void handleRequest() {
    try {
      if (m_allowedMethods.at(m_request.method())) {
        switch (m_request.method()) {
        case boost::beast::http::verb::post:
          doPost();
          break;
        case boost::beast::http::verb::get:
          doGet();
          break;
        case boost::beast::http::verb::put:
          doPut();
          break;
        case boost::beast::http::verb::patch:
          doPatch();
          break;
        case boost::beast::http::verb::delete_:
          doDelete();
          break;
        default:
          writeNotImplementedResponse();
        }
      } else {
        writeMethodNotAllowed();
      }
    } catch (std::out_of_range) {
      writeMethodNotAllowed();
    }
  }

public:
  /**
   * HTTP Endpoint where all method is not allowed by default
   */
  /*HttpRestrictiveEndpoint(
      boost::beast::http::request<
          Body, boost::beast::http::basic_fields<Allocator>> &&req,
      Send &&send, bool allowPost = false, bool allowGet = false,
      bool allowPut = false, bool allowPatch = false, bool allowDelete = false)
      : m_request(req), m_send(send) {

    m_allowedMethods = {{boost::beast::http::verb::post, allowPost},
                        {boost::beast::http::verb::get, allowGet},
                        {boost::beast::http::verb::put, allowPut},
                        {boost::beast::http::verb::patch, allowPatch},
                        {boost::beast::http::verb::delete_, allowDelete}};
    handleRequest();
  };*/
  /**
   * HTTP Endpoint where all method is not allowed by default
   */
  HttpRestrictiveEndpoint(
      boost::beast::http::request<
          Body, boost::beast::http::basic_fields<Allocator>> &&req,
      Send &&send, std::map<boost::beast::http::verb, bool> allowedMethods)
      : m_request(req), m_send(send), m_allowedMethods(allowedMethods) {

    handleRequest();
  };

  ~HttpRestrictiveEndpoint() {}
};

#endif // __HTTP_RESTRICTIVE_ENDPOINT_HPP__