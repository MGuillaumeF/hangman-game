#ifndef __HTTP_TOKEN_ENDPOINT_HPP__
#define __HTTP_TOKEN_ENDPOINT_HPP__
#include "../common/HTTP/RestrictiveEndpoint.hpp"

/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpTokenEndpoint final : public http::RestrictiveEndpoint {

public:
  /**
   * @brief Construct a new Http Token Endpoint object
   * @param req The request to handle
   */
  explicit HttpTokenEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req);
  /**
   * @brief Method to overwrite POST method
   */
  void doPost() override;
  
private:
  /**
   * @brief Get logger
   */
  std::unique_ptr<Logger> &m_logger = Logger::getInstance();
};

#endif // __HTTP_TOKEN_ENDPOINT_HPP__
