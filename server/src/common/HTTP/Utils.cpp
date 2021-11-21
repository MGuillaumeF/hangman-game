#include "Utils.hpp"
#include "../Logger/Logger.hpp"
#include <boost/property_tree/xml_parser.hpp>

namespace http {
/**
 * @brief configuration map of mimetype where key is extension, populated at the
 * first mime-type search
 */
std::map<std::string, std::string, std::less<>> Utils::s_extTomimtype = {};

/**
 * @brief The default constructor of Utils class
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

  // set default mime type
  std::string l_sMimeType = "application/text";
  // get logger for trace
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  // if mime type configuration map is empty log configuration file
  if (Utils::s_extTomimtype.empty()) {
    Utils::loadMimTypesConfiguration();
  }
  // get the mime type from configuration if ext is found in loaded
  // configuration
  if (Utils::s_extTomimtype.contains(ext.to_string())) {
    l_sMimeType = Utils::s_extTomimtype.at(ext.to_string());
  } else {
    logger->warn("HTTP_CONFIGURATION",
                 "MimeType of File not found for extension : " +
                     ext.to_string());
  }
  logger->debug("HTTP_CONFIGURATION", "MimeType of File used : " + l_sMimeType);
  return l_sMimeType;
}

/**
 * @brief Load Mim Types Configuration map
 */
void Utils::loadMimTypesConfiguration() {
  // get logger for trace
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  logger->debug("HTTP_CONFIGURATION",
                "MimeType mapper is empty, readding configuration file");
  boost::property_tree::ptree mimeTypesConfig;
  // parse configuration file as xml representation of preperty tree
  boost::property_tree::read_xml("./configuration/mime-types.xml",
                                 mimeTypesConfig);
  // for each mime-type row get extension and associated mime type value
  for (const std::pair<const std::string, boost::property_tree::ptree>
           &mimeType : mimeTypesConfig.get_child("mime-types")) {
    const std::string l_ext = mimeType.second.get<std::string>("<xmlattr>.ext");
    const std::string l_value =
        mimeType.second.get<std::string>("<xmlattr>.value");
    logger->debug("HTTP_CONFIGURATION",
                  "MimeType pair found [" + l_ext + ", " + l_value + "]");
    // insert value in configuration map
    s_extTomimtype.try_emplace(l_ext, l_value);
  }
  logger->debug("HTTP_CONFIGURATION",
                "MimeType mapper loaded for " +
                    std::to_string(Utils::s_extTomimtype.size()) +
                    " extensions");
}

/**
 * @brief The default destructor of Utils class
 */
Utils::~Utils() = default;

} // namespace http
