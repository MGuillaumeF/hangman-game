#ifndef __HTTP_LOCATION_ENDPOINT_HPP__
#define __HTTP_LOCATION_ENDPOINT_HPP__
#include "RestrictiveEndpoint.hpp"

/**
 * Http Endpoint Class to create HTTP response
 */
class LocationEndpoint : public http::RestrictiveEndpoint {

public:
  /**
   * @brief Construct a new Location Endpoint object
   *
   * @param req The request to handle
   * @param rootDirectory The start directory of static files hosting
   */
  explicit LocationEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const std::string_view &rootDirectory);
  /**
   * @brief Methode to read file with GET methode
   *
   */
  void doGet() override;

  /**
   * @brief Methode to delete file with DELETE methode
   *
   */
  void doDelete() override;

  /**
   * @brief Destroy the Http Token Endpoint object
   */
  ~LocationEndpoint() final;

private:
  /**
   * @brief Get logger
   */
  std::unique_ptr<Logger> &m_logger = Logger::getInstance();

  std::string m_rootDirectory;
  /**
   * @brief Append an HTTP rel-path to a local filesystem path.
   *
   * @param base The base of path to merge
   * @param path The end of path to merge
   * @return std::string  The returned path is normalized for the platform.
   */
  static std::string pathCat(const boost::beast::string_view &base,
                             const boost::beast::string_view &path);
};

#endif // __HTTP_LOCATION_ENDPOINT_HPP__
