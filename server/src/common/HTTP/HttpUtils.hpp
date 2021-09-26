#ifndef __HTTP_UTILS_H__
#define __HTTP_UTILS_H__

#include <boost/beast/core.hpp>

#include "../Logger/LoggerFile.hpp"
/**
 * Utilitaries functions in static class
 */
class HttpUtils {
private:
  /**
   * Output file path
   */
  static const std::string s_outputFilePath;

public:
  /**
   * Application LoggerFile
   */
  static const LoggerFile s_appLogger;
  /**
   * The default constructor of Utils class
   */
  HttpUtils();

  /**
   *  Return a reasonable mime type based on the extension of a file.
   * @param path the path of request
   * @return the mime-type of file
   */
  static boost::beast::string_view getMimeType(boost::beast::string_view path);

  /**
   * Report a failure
   * @param ec the error code
   * @param what the explanation of error
   */
  static void onFail(boost::beast::error_code ec, char const *what);

  /**
   * The default destructor of Utils class
   */
  ~HttpUtils();
};

#endif