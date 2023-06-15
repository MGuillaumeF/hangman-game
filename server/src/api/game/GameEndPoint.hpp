#ifndef __GAME_ENDPOINT_HPP__
#define __GAME_ENDPOINT_HPP__
#include "../../common/HTTP/RestrictiveEndpoint.hpp"

/**
 * Http Endpoint Class ito create HTTP response
 */
class GameEndPoint final : public http::RestrictiveEndpoint {

public:
  /**
   * @brief Construct a new Http Token Endpoint object
   * @param req The request to handle
   */
  explicit GameEndPoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req);
  /**
   * @brief Method to overwrite POST method
   */
  void doPost() override;
  /**
   * @brief Destroy the Http Token Endpoint object
   */
  ~GameEndPoint() override;

private:
  /**
   * @brief Get logger
   */
  std::unique_ptr<Logger> &m_logger = Logger::getInstance();
};

#endif // __GAME_ENDPOINT_HPP__
