#include "HttpUtils.hpp"

/**
 * The default constructor of Utils class
 */
HttpUtils::HttpUtils() {}

/**
 *  Return a reasonable mime type based on the extension of a file.
 * @param path the path of request
 * @return the mime-type of file
 */
boost::beast::string_view
HttpUtils::getMimeType(boost::beast::string_view path) {
  using boost::beast::iequals;
  auto const ext = [&path] {
    auto const pos = path.rfind(".");
    if (pos == boost::beast::string_view::npos)
      return boost::beast::string_view{};
    return path.substr(pos);
  }();

  boost::beast::string_view l_sMimeType = "application/text";

  if (iequals(ext, ".htm") || iequals(ext, ".html") || iequals(ext, ".php")) {
    l_sMimeType = "text/html";
  } else if (iequals(ext, ".css")) {
    l_sMimeType = "text/css";
  } else if (iequals(ext, ".txt")) {
    l_sMimeType = "text/plain";
  } else if (iequals(ext, ".js")) {
    l_sMimeType = "application/javascript";
  } else if (iequals(ext, ".json")) {
    l_sMimeType = "application/json";
  } else if (iequals(ext, ".xml")) {
    l_sMimeType = "application/xml";
  } else if (iequals(ext, ".swf")) {
    l_sMimeType = "application/x-shockwave-flash";
  } else if (iequals(ext, ".flv")) {
    l_sMimeType = "video/x-flv";
  } else if (iequals(ext, ".png")) {
    l_sMimeType = "image/png";
  } else if (iequals(ext, ".jpe") || iequals(ext, ".jpeg") ||
             iequals(ext, ".jpg")) {
    l_sMimeType = "image/jpeg";
  } else if (iequals(ext, ".gif")) {
    l_sMimeType = "image/gif";
  } else if (iequals(ext, ".bmp")) {
    l_sMimeType = "image/bmp";
  } else if (iequals(ext, ".ico")) {
    l_sMimeType = "image/vnd.microsoft.icon";
  } else if (iequals(ext, ".tiff") || iequals(ext, ".tif")) {
    l_sMimeType = "image/tiff";
  } else if (iequals(ext, ".svg") || iequals(ext, ".svgz")) {
    l_sMimeType = "image/svg+xml";
  }
  LoggerFile logger = LoggerFile();
  logger.info("MimeType of File found : " + l_sMimeType.to_string());
  return l_sMimeType;
}

/**
 * Report a failure
 * @param ec the error code
 * @param what the explanation of error
 */
void HttpUtils::onFail(boost::beast::error_code ec, char const *what) {
  LoggerFile logger = LoggerFile();
  logger.error(std::string(what) + ": " + ec.message());
}

/**
 *
 * The default destructor of Utils class
 */
HttpUtils::~HttpUtils() {}