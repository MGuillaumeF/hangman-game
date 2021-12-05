#include "ConfigurationServer.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../../Logger/Logger.hpp"
#include "../Exception/ConfigurationException.hpp"

/**
 * @brief Construct a new Configuration Server:: Configuration Server object
 *
 * @param argv arguments list of server configuration
 */
ConfigurationServer::ConfigurationServer(const std::vector<std::string>& argv) {

  const std::unique_ptr<Logger>& logger = Logger::getInstance();
  // if arguments list is empty, read configuration file
  if (argv.empty()) {
    boost::property_tree::ptree configuration_properties;
    // read default configuration file
    boost::property_tree::read_xml("./configuration/servers.xml", configuration_properties);

    try {
      // save configuration of server
      m_description = configuration_properties.get<std::string>("servers.server.description");
      m_hostname = configuration_properties.get<std::string>("servers.server.hostname");
      m_root = configuration_properties.get<std::string>("servers.server.endpoints.endpoint.root-directory");
      m_thread = configuration_properties.get<uint8_t>("servers.server.thread");
      m_port = configuration_properties.get<uint16_t>("servers.server.port");

      logger->info("HTTP_CONFIGURATION",
                   "Configuration reading : "
                   "\n -> Description  : " +
                       m_description + "\n -> Hostname     : " + m_hostname +
                       "\n -> Port         : " + std::to_string(m_port) +
                       "\n -> Root         : " + m_root +
                       "\n -> Thread       : " + std::to_string(m_thread));

    } catch (const boost::property_tree::ptree_error &e) {
      logger->error("HTTP_CONFIGURATION",
                  std::string("Configuration reading error") +
                      std::string(e.what()));
      throw ConfigurationException("HTTP Configuration file is invalid");
    }
  }
}
/**
 * @brief  Get the Description object
 *
 * @return std::string
 */
std::string ConfigurationServer::getDescription() const {
  return m_description;
};
/**
 * @brief Get the Hostname object
 *
 * @return std::string
 */
std::string ConfigurationServer::getHostname() const { return m_hostname; };

/**
 * @brief Get the Root object
 *
 * @return uint16_t
 */
std::string ConfigurationServer::getRoot() const { return m_root; };

/**
 * @brief Get the Threads object
 *
 * @return uint16_t
 */
uint8_t ConfigurationServer::getThreads() const { return m_thread; };

/**
 * @brief Get the Port object
 *
 * @return uint16_t
 */
uint16_t ConfigurationServer::getPort() const { return m_port; };
