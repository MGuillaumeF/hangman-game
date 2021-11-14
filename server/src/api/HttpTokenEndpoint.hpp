#ifndef __HTTP_TOKEN_ENDPOINT_HPP__
#define __HTTP_TOKEN_ENDPOINT_HPP__
#include "../common/HTTP/RestrictiveEndpoint.hpp"

/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpTokenEndpoint : public http::RestrictiveEndpoint {

public:
  explicit HttpTokenEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req);
  void doPost() override;
  ~HttpTokenEndpoint();
private:
  std::unique_ptr<Logger>& m_logger = Logger::getInstance();

};

#endif // __HTTP_TOKEN_ENDPOINT_HPP__
