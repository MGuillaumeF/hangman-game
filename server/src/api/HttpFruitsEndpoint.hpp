#ifndef __HTTP_FRUITS_ENDPOINT_HPP__
#define __HTTP_FRUITS_ENDPOINT_HPP__
#include "HttpRestrictiveEndpoint.hpp"
#include <iostream>
/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpFruitsEndpoint : public HttpRestrictiveEndpoint {

public:
  HttpFruitsEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req)
      : HttpRestrictiveEndpoint(req,
                                {{boost::beast::http::verb::post, false},
                                 {boost::beast::http::verb::get, true},
                                 {boost::beast::http::verb::put, false},
                                 {boost::beast::http::verb::patch, false},
                                 {boost::beast::http::verb::delete_, false}}) {

    std::cout << "HttpFruitsEndpoint - constructor" << std::endl;
  }
  void doGet() override {
    std::cout << "HttpFruitsEndpoint - doGet - start" << std::endl;
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::ok, m_request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = "<html><head></head><body>This is a response</body></html>";
    res.prepare_payload();
    setResponse(res);
    std::cout << "HttpFruitsEndpoint - doGet - end" << std::endl;
  }
};

#endif // __HTTP_FRUITS_ENDPOINT_HPP__