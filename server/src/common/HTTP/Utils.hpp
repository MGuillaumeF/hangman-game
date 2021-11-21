#ifndef __HTTP_UTILS_HPP__
#define __HTTP_UTILS_HPP__

#include <boost/beast/core.hpp>
#include <map>
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
  static boost::beast::string_view
  getMimeType(const boost::beast::string_view &path);

  /**
   * @brief Load Mim Types Configuration map
   */
  static void loadMimTypesConfiguration();

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
  static std::map<std::string, std::string> s_extTomimtype;
};

} // namespace http
#endif
