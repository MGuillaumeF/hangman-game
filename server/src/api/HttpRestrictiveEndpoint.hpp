#ifndef __HTTP_RESTRICTIVE_ENDPOINT_HPP__
#define __HTTP_RESTRICTIVE_ENDPOINT_HPP__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <map>

/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpRestrictiveEndpoint {
private:
  virtual void doGet() {
    std::cout << "HttpRestrictiveEndpoint - doGet - start" << std::endl;
    writeNotImplementedResponse();
    std::cout << "HttpRestrictiveEndpoint - doGet - end" << std::endl;
  }

  virtual void doPost() { writeNotImplementedResponse(); }

  virtual void doPut() { writeNotImplementedResponse(); }

  virtual void doDelete() { writeNotImplementedResponse(); }

  virtual void doPatch() { writeNotImplementedResponse(); }

protected:
  boost::beast::http::request<boost::beast::http::string_body> m_request;
  boost::beast::http::response<boost::beast::http::string_body> m_response;
  std::map<boost::beast::http::verb, bool> m_allowedMethods;

  void writeMethodNotAllowed() {
    boost::beast::http::response<boost::beast::http::string_body> res;

    res.version(11); // HTTP/1.1
    res.result(boost::beast::http::status::method_not_allowed);
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = "The method is not allowed.";
    res.prepare_payload();
    m_response = res;
  }

  void writeNotImplementedResponse() {
    boost::beast::http::response<boost::beast::http::string_body> res;

    res.version(11); // HTTP/1.1
    res.result(boost::beast::http::status::not_implemented);
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = "The method is not implemented.";
    res.prepare_payload();
    m_response = res;
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
    dispatchRequest();
  };*/
  /**
   * HTTP Endpoint where all method is not allowed by default
   */
  HttpRestrictiveEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      std::map<boost::beast::http::verb, bool> allowedMethods)
      : m_request(req), m_allowedMethods(allowedMethods){};
  void dispatchRequest() {
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
  void setResponse(
      boost::beast::http::response<boost::beast::http::string_body> response) {
    m_response = response;
  }
  boost::beast::http::response<boost::beast::http::string_body> getResponse() {
    std::cout << "getResponse - start " << std::endl
              << "body : " << m_response.body() << std::endl;
    return m_response;
    std::cout << "getResponse - end " << std::endl;
  }

  ~HttpRestrictiveEndpoint() {}
};

#endif // __HTTP_RESTRICTIVE_ENDPOINT_HPP__