#ifndef __HTTP_UTILS_HPP__
#define __HTTP_UTILS_HPP__

#include <boost/beast/core.hpp>
#include <map>

namespace http {
/**
 * Utilitaries functions in static class
 */
class Utils {
public:
  /**
   * The default constructor of Utils class
   */
  Utils();

  /**
   * @brief Return a reasonable mime type based on the extension of a file.
   * @param path the path of request
   * @return the mime-type of file
   */
  static boost::beast::string_view
  getMimeType(const boost::beast::string_view &path);

  /**
   * The default destructor of Utils class
   */
  ~Utils();

private:
  /**
   * Output file path
   */
  static const std::map<boost::beast::string_view, boost::beast::string_view>
      s_extTomimtype;
};

} // namespace HTTP
#endif
