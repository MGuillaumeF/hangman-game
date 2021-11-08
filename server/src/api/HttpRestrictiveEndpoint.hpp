#ifndef __HTTP_RESTRICTIVE_ENDPOINT_HPP__
#define __HTTP_RESTRICTIVE_ENDPOINT_HPP__

#include <iostream>
#include <map>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "../common/Logger/Logger.hpp"

/**
 * Http Endpoint Class to create HTTP response
 */
class HttpRestrictiveEndpoint {

public:
  /**
   * @brief Construct a new Http Restrictive Endpoint object - HTTP Endpoint
   * where all method is not allowed by default
   *
   * @param req The request received at this endpoint
   * @param allowedMethods The allowed method paramaters object
   */
  HttpRestrictiveEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const std::map<boost::beast::http::verb, bool> &allowedMethods)
      : m_request(req), m_allowedMethods(allowedMethods){};
  /**
   * @brief Method to dispatch work by HTTP method
   *
   */
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
    } catch (const std::out_of_range &e) {
      m_logger->error(
          "HTTP_DATA_READ",
          "HttpRestrictiveEndpoint - dispatchRequest - out of range of method" +
              std::string(e.what()));
      writeMethodNotAllowed();
    }
  }
  /**
   * @brief Set the Response object
   *
   * @param response
   */
  void setResponse(
      const boost::beast::http::response<boost::beast::http::string_body>
          &response) {
    m_response = response;
  }
  /**
   * @brief Get the Response object
   *
   * @return boost::beast::http::response<boost::beast::http::string_body>
   */
  boost::beast::http::response<boost::beast::http::string_body>
  getResponse() const {
    m_logger->debug("HTTP_DATA_READ",
                    "HttpRestrictiveEndpoint - getResponse - start");
    m_logger->debug("HTTP_DATA_READ",
                    "HttpRestrictiveEndpoint - readResponse body " +
                        m_response.body());
    m_logger->debug("HTTP_DATA_READ",
                    "HttpRestrictiveEndpoint - getResponse - end");
    return m_response;
  }
  /**
   * @brief Get the Request object
   *
   * @return boost::beast::http::request<boost::beast::http::string_body>
   */
  boost::beast::http::request<boost::beast::http::string_body>
  getRequest() const {
    return m_request;
  }

  /**
   * @brief Method to check if HTTP methode is allowed
   *
   * @param method The HTTP method to test
   * @return true - The HTTP method is allowed
   * @return false - The HTTP method is not allowed
   */
  bool methodIsAllowed(const boost::beast::http::verb &method) const {
    return m_allowedMethods.at(method);
  }

  /**
   * @brief Destroy the Http Restrictive Endpoint object
   *
   */
  virtual ~HttpRestrictiveEndpoint() = default;

protected:
  /**
   * @brief Method not Allowed default error
   *
   */
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
  /**
   * @brief Not Implemented result
   *
   */
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

private:
  /**
   * @brief logger singleton instance
   *
   */
   const std::unique_ptr<Logger>& m_logger = Logger::getInstance();

  /**
   * @brief The HTTP Request
   *
   */
  boost::beast::http::request<boost::beast::http::string_body> m_request;
  /**
   * @brief The HTTP Response
   *
   */
  boost::beast::http::response<boost::beast::http::string_body> m_response;

  /**
   * @brief The Map of HTTP Allowed Method
   *
   */
  std::map<boost::beast::http::verb, bool> m_allowedMethods;

  /**
   * @brief DEFAULT - GET - HTTP Method not implemented for endpoind
   */
  virtual void doGet() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doGet - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doGet - end");
  }

  /**
   * @brief DEFAULT - POST - HTTP Method not implemented for endpoind
   */
  virtual void doPost() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPost - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPost - end");
  }

  /**
   * @brief DEFAULT - PUT - HTTP Method not implemented for endpoind
   */
  virtual void doPut() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPut - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPut - end");
  }

  /**
   * @brief DEFAULT - DELETE - HTTP Method not implemented for endpoind
   */
  virtual void doDelete() {
    m_logger->debug("HTTP_ACCESS",
                    "HttpRestrictiveEndpoint - doDelete - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doDelete - end");
  }

  /**
   * @brief DEFAULT - PATCH - HTTP Method not implemented for endpoind
   */
  virtual void doPatch() {
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPatch - start");
    writeNotImplementedResponse();
    m_logger->debug("HTTP_ACCESS", "HttpRestrictiveEndpoint - doPatch - end");
  }
};

#endif // __HTTP_RESTRICTIVE_ENDPOINT_HPP__
