#ifndef __HTTP_LOCATION_ENDPOINT_HPP__
#define __HTTP_LOCATION_ENDPOINT_HPP__
#include "RestrictiveEndpoint.hpp"

/**
 * Http Endpoint Class to create HTTP response
 */
class LocationEndpoint final : public http::RestrictiveEndpoint {

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
   * @brief Method to read file with GET method
   *
   */
  void doGet() override;

  /**
   * @brief Method to delete file with DELETE method
   *
   */
  void doDelete() override;

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

  /**
   * @brief Function to test if targat of request is a valid path
   *
   * @param boost::beast::string_view The target of request
   * @return true The target of request is valid relative path (desc)
   * @return false The target have bad structure of want to go up on file tree
   * and it's forbidden
   */
  static bool isValidFileTarget(const boost::beast::string_view &target);
};

#endif // __HTTP_LOCATION_ENDPOINT_HPP__
