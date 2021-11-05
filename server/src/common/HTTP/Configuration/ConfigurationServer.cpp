#include "ConfigurationServer.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../../Logger/Logger.hpp"
#include "../Exception/ConfigurationException.hpp"

/**
 * @brief Construct a new Configuration Server:: Configuration Server object
 *
 * @param argv
 */
ConfigurationServer::ConfigurationServer(char *argv[]) {

  Logger *logger = Logger::getInstance();

  boost::property_tree::ptree pt;
  boost::property_tree::read_xml("../resources/configuration.xml", pt);

  try {
    m_description = pt.get<std::string>("server.description");
    m_hostname = pt.get<std::string>("server.hostname");
    m_root = pt.get<std::string>("server.root");
    m_thread = pt.get<unsigned short>("server.thread");
    m_port = pt.get<uint16_t>("server.port");

    logger->info("HTTP_CONFIGURATION",
                 "Configuration reading : "
                 "\n -> Description  : " +
                     m_description + "\n -> Hostname     : " + m_hostname +
                     "\n -> Port         : " + std::to_string(m_port) +
                     "\n -> Root         : " + m_root +
                     "\n -> Thread       : " + std::to_string(m_thread));

  } catch (const boost::property_tree::ptree_error &e) {
    logger->error("HTTP_CONFIGURATION", std::string("Configuration reading error") + std::string(e.what()));
    throw ConfigurationException("HTTP Configuration file is invalid");
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
