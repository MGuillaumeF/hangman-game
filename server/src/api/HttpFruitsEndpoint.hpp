#ifndef __HTTP_FRUITS_ENDPOINT_HPP__
#define __HTTP_FRUITS_ENDPOINT_HPP__
#include "../common/Logger/Logger.hpp"
#include "HttpRestrictiveEndpoint.hpp"
#include <iostream>
/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpFruitsEndpoint : public HttpRestrictiveEndpoint {

public:
  explicit HttpFruitsEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req)
      : HttpRestrictiveEndpoint(req,
                                {{boost::beast::http::verb::post, false},
                                 {boost::beast::http::verb::get, true},
                                 {boost::beast::http::verb::put, false},
                                 {boost::beast::http::verb::patch, false},
                                 {boost::beast::http::verb::delete_, false}}) {

    m_logger->debug("HTTP_ACCESS", "HttpFruitsEndpoint - constructor");
  }
  void doGet() override {
    m_logger->debug("HTTP_ACCESS", "HttpFruitsEndpoint - doGet - start");
    const boost::beast::http::request<boost::beast::http::string_body> request =
        this->getRequest();

    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::ok, request.version()};

    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(request.keep_alive());
    res.body() = "<html><head></head><body>This is a response</body></html>";
    res.prepare_payload();
    setResponse(res);
    m_logger->debug("HTTP_ACCESS", "HttpFruitsEndpoint - doGet - end");
  }

private:
  const std::unique_ptr<Logger> m_logger = Logger::getInstance();

};

#endif // __HTTP_FRUITS_ENDPOINT_HPP__
