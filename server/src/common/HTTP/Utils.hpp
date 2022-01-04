#ifndef __HTTP_UTILS_HPP__
#define __HTTP_UTILS_HPP__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/property_tree/ptree.hpp>
#include <functional>
#include <map>
#include <string>
namespace http {
/**
 * @brief Utilitaries functions in static class
 */
class Utils {
public:
  /**
   * @brief Return a reasonable mime type based on the extension of a file.
   * @param path the path of request
   * @return the mime-type of file
   */
  static std::string_view
  getMimeType(const std::string_view &path);

  /**
   * @brief Load Mim Types Configuration map
   */
  static void loadMimTypesConfiguration();

  /**
   * @brief Get the Tree object of Body request
   *
   * @param req The request of client
   * @return boost::property_tree::ptree The property tree of body content
   */
  static boost::property_tree::ptree getBodyTree(
      const boost::beast::http::request<boost::beast::http::string_body> &req);

  /**
   * @brief static method of default bad_request response
   *
   * @param req The request of client
   * @param why The reason of bad request
   * @return boost::beast::http::response<boost::beast::http::string_body> The
   * body response
   */
  static boost::beast::http::response<boost::beast::http::string_body>
  bad_request(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const std::string_view &why);

  /**
   * @brief static method of default not_found response
   *
   * @param req The request of client
   * @param target The target not found
   * @return boost::beast::http::response<boost::beast::http::string_body> The
   * body response
   */
  static boost::beast::http::response<boost::beast::http::string_body>
  not_found(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const boost::beast::string_view &target);

  /**
   * @brief static method of default server_error response
   *
   * @param req The request of client
   * @param what The server error message
   * @return boost::beast::http::response<boost::beast::http::string_body> The
   * body response
   */
  static boost::beast::http::response<boost::beast::http::string_body>
  server_error(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const boost::beast::string_view &what);

  /**
   * @brief The response writer wrapper
   *
   * @param req The request of client
   * @param status The status of response HTTP
   * @param body The body of response
   * @param contentType The content type of body response
   * @return boost::beast::http::response<boost::beast::http::string_body>
   */
  static boost::beast::http::response<boost::beast::http::string_body>
  wrapper_response(
      const boost::beast::http::request<boost::beast::http::string_body> &req,
      const boost::beast::http::status &status,
      const std::string_view &body,
      const std::string_view &contentType = "text/html");

private:
  /**
   * @brief The default constructor of Utils class
   */
  Utils();
  /**
   * @brief The default destructor of Utils class
   */
  ~Utils();
  /**
   * @brief configuration map of mimetype where key is extension, populated at
   * the first mime-type search
   */
  static std::map<std::string, std::string, std::less<>> s_extTomimtype;
};

} // namespace http
#endif
