#ifndef __HTTP_LOCATION_ENDPOINT_HPP__
#define __HTTP_LOCATION_ENDPOINT_HPP__
#include "RestrictiveEndpoint.hpp"

/**
 * Http Endpoint Class to create HTTP response
 */
class LocationEndpoint : public http::RestrictiveEndpoint {

public:
  /**
   * @brief Construct a new Http Token Endpoint object
   * @param req The request to handle
   */
  explicit LocationEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const std::string &rootDirectory);
  /**
   * @brief Methode to read file with GET methode
   *
   */
  void doGet() override;

  /**
   * @brief Methode to upload file with POST methode
   *
   */
  void doPost() override;

  /**
   * @brief Methode to replace file with PUT methode
   *
   */
  void doPut() override;

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
  std::string pathCat(const boost::beast::string_view &base,
                      const boost::beast::string_view &path) const;
};

#endif // __HTTP_LOCATION_ENDPOINT_HPP__
