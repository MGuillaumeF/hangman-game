#include "Utils.hpp"
#include "../Logger/Logger.hpp"

namespace http {
const std::map<boost::beast::string_view, boost::beast::string_view>
    Utils::s_extTomimtype = {{".htm", "text/html"},
                             {".html", "text/html"},
                             {".php", "text/html"},
                             {".css", "text/css"},
                             {".txt", "text/plain"},
                             {".js", "application/javascript"},
                             {".json", "application/json"},
                             {".xml", "application/xml"},
                             {".swf", "application/x-shockwave-flash"},
                             {".flv", "video/x-flv"},
                             {".png", "image/png"},
                             {".jpe", "image/jpeg"},
                             {".jpeg", "image/jpeg"},
                             {".jpg", "image/jpeg"},
                             {".gif", "image/gif"},
                             {".bmp", "image/bmp"},
                             {".ico", "image/vnd.microsoft.icon"},
                             {".tiff", "image/tiff"},
                             {".tif", "image/tiff"},
                             {".svg", "image/svg+xml"},
                             {".svgz", "image/svg+xml"}};

/**
 * The default constructor of Utils class
 */
Utils::Utils() = default;

/**
 * @brief Return a reasonable mime type based on the extension of a file.
 * @param path the path of request
 * @return the mime-type of file
 */
boost::beast::string_view
Utils::getMimeType(const boost::beast::string_view &path) {
  using boost::beast::iequals;
  auto const ext = [&path] {
    auto const pos = path.rfind(".");
    return (pos == boost::beast::string_view::npos)
               ? boost::beast::string_view{}
               : path.substr(pos);
  }();

  boost::beast::string_view l_sMimeType = "application/text";
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  try {
    l_sMimeType = Utils::s_extTomimtype.at(ext);
  } catch (const std::out_of_range &) {
    logger->warn("HTTP_CONFIGURATION",
                 "MimeType of File not found for extension : " +
                     ext.to_string());
  }
  logger->info("HTTP_CONFIGURATION",
               "MimeType of File found : " + l_sMimeType.to_string());
  return l_sMimeType;
}

/**
 *
 * The default destructor of Utils class
 */
Utils::~Utils() = default;

} // namespace HTTP
