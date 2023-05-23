#ifndef __HTTP_RESTRICTIVE_ENDPOINT_HPP__
#define __HTTP_RESTRICTIVE_ENDPOINT_HPP__

#include <set>

#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "../Logger/Logger.hpp"

const uint8_t HTTP_VERSION = 11;

namespace http {

/**
 * Http Endpoint Class to create HTTP response
 */
class RestrictiveEndpoint {

public:
  /**
   * @brief Construct a new Http Restrictive Endpoint object - HTTP Endpoint
   * where all method is not allowed by default
   *
   * @param req The request received at this endpoint
   * @param allowedMethods The allowed method paramaters object
   */
  RestrictiveEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const std::set<boost::beast::http::verb> &allowedMethods)
      : m_request(req), m_allowedMethods(allowedMethods){};
  /**
   * @brief Method to dispatch work by HTTP method
   *
   */
  void dispatchRequest() {
    if (m_allowedMethods.contains(m_request.method())) {
      switch (m_request.method()) {
      case boost::beast::http::verb::post:
        // method is POST
        doPost();
        break;
      case boost::beast::http::verb::get:
        // method is GET
        doGet();
        break;
      case boost::beast::http::verb::put:
        // method is PUT
        doPut();
        break;
      case boost::beast::http::verb::patch:
        // method is PATCH
        doPatch();
        break;
      case boost::beast::http::verb::delete_:
        // method is DELETE
        doDelete();
        break;
      default:
        // not possible
        writeNotImplementedResponse();
      }
    } else {
      // if method is unknown
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
  [[nodiscard]] boost::beast::http::response<boost::beast::http::string_body>
  getResponse() const {
    return m_response;
  }
  /**
   * @brief Get the Request object
   *
   * @return boost::beast::http::request<boost::beast::http::string_body>
   */
  [[nodiscard]] boost::beast::http::request<boost::beast::http::string_body>
  getRequest() const {
    return m_request;
  }

  /**
   * @brief Method to check if HTTP method is allowed
   *
   * @param method The HTTP method to test
   * @return true - The HTTP method is allowed
   * @return false - The HTTP method is not allowed
   */
  [[nodiscard]] bool
  methodIsAllowed(const boost::beast::http::verb &method) const {
    return m_allowedMethods.contains(method);
  }

protected:
  /**
   * @brief Method not Allowed default error
   *
   */
  void writeMethodNotAllowed() {
    // write method is not allowed http response
    boost::beast::http::response<boost::beast::http::string_body> res;

    res.version(HTTP_VERSION); // HTTP/1.1
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
    // write not implemented http response
    boost::beast::http::response<boost::beast::http::string_body> res;

    res.version(HTTP_VERSION); // HTTP/1.1
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
  const std::unique_ptr<Logger> &m_logger = Logger::getInstance();

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
  std::set<boost::beast::http::verb> m_allowedMethods;

  /**
   * @brief DEFAULT - GET - HTTP Method not implemented for endpoind
   */
  virtual void doGet() {
    // if doGet is not overwrited return not implemented response
    writeNotImplementedResponse();
  }

  /**
   * @brief DEFAULT - POST - HTTP Method not implemented for endpoind
   */
  virtual void doPost() {
    // if doPost is not overwrited return not implemented response
    writeNotImplementedResponse();
  }

  /**
   * @brief DEFAULT - PUT - HTTP Method not implemented for endpoind
   */
  virtual void doPut() {
    // if doPut is not overwrited return not implemented response
    writeNotImplementedResponse();
  }

  /**
   * @brief DEFAULT - DELETE - HTTP Method not implemented for endpoind
   */
  virtual void doDelete() {
    // if doDelete is not overwrited return not implemented response
    writeNotImplementedResponse();
  }

  /**
   * @brief DEFAULT - PATCH - HTTP Method not implemented for endpoind
   */
  virtual void doPatch() {
    // if doPatch is not overwrited return not implemented response
    writeNotImplementedResponse();
  }
};

} // namespace http
#endif // __HTTP_RESTRICTIVE_ENDPOINT_HPP__
