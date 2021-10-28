#ifndef __HTTP_RESTRICTIVE_ENDPOINT_HPP__
#define __HTTP_RESTRICTIVE_ENDPOINT_HPP__

#include <iostream>
#include <map>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "../common/Logger/Logger.hpp"

/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpRestrictiveEndpoint {
private:
  Logger *m_logger = Logger::getInstance();
  virtual void doGet() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doGet - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doGet - end");
  }

  virtual void doPost() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPost - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPost - end");
  }

  virtual void doPut() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPut - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPut - end");
  }

  virtual void doDelete() {
    m_logger->debug("HTTP_ACCESS",
                    "HttpRestrictiveEndpoint - doDelete - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doDelete - end");
  }

  virtual void doPatch() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPatch - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPatch - end");
  }

  boost::beast::http::request<boost::beast::http::string_body> m_request;
  boost::beast::http::response<boost::beast::http::string_body> m_response;
  std::map<boost::beast::http::verb, bool> m_allowedMethods;

protected:

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
      m_logger->error(
          "HTTP_DATA_READ",
          "HttpRestrictiveEndpoint - dispatchRequest - out of range of method");
      writeMethodNotAllowed();
    }
  }
  void setResponse(
      boost::beast::http::response<boost::beast::http::string_body> response) {
    m_response = response;
  }
  boost::beast::http::response<boost::beast::http::string_body> getResponse() {
    m_logger->debug("HTTP_DATA_READ",
                    "HttpRestrictiveEndpoint - getResponse - start");
    m_logger->debug("HTTP_DATA_READ",
                    "HttpRestrictiveEndpoint - readResponse body " +
                        m_response.body());
    m_logger->debug("HTTP_DATA_READ",
                    "HttpRestrictiveEndpoint - getResponse - end");
    return m_response;
  }

  ~HttpRestrictiveEndpoint() {}
};

#endif // __HTTP_RESTRICTIVE_ENDPOINT_HPP__
