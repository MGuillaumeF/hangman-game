#ifndef __HTTP_FRUITS_ENDPOINT_HPP__
#define __HTTP_FRUITS_ENDPOINT_HPP__
#include "HttpRestrictiveEndpoint.hpp"

/**
 * Http Endpoint Class ito create HTTP response
 */
template <class Body, class Allocator, class Send>
class HttpFruitsEndpoint : public HttpRestrictiveEndpoint {

public:
  HttpFruitsEndpoint(
      boost::beast::http::request<
          Body, boost::beast::http::basic_fields<Allocator>> &&req,
      Send &&send, std::map<boost::beast::http::verb, bool> allowedMethods)
      : HttpRestrictiveEndpoint(req, send, allowedMethods) {}
  void doGet() {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::ok, m_request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());
    res.body() = "This is a response";
    res.prepare_payload();
    m_send(res);
  }
};

#endif // __HTTP_FRUITS_ENDPOINT_HPP__