#include "ConfigurationServer.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../../Logger/Logger.hpp"
#include "../Exception/ConfigurationException.hpp"

ConfigurationServer::ConfigurationServer(char *argv[]) {

  Logger *logger = Logger::getInstance();

  boost::property_tree::ptree pt;
  boost::property_tree::read_xml("../resources/configuration.xml", pt);

  try {
    m_description = pt.get<std::string>("server.description");
    m_hostname = pt.get<std::string>("server.hostname");
    m_root = pt.get<std::string>("server.root");
    m_thread = pt.get<unsigned short>("server.thread");
    m_port = pt.get<unsigned>("server.port");

    logger->info("HTTP_CONFIGURATION",
                 "Configuration reading : "
                 "\n -> Description  : " +
                     m_description + "\n -> Hostname     : " + m_hostname +
                     "\n -> Port         : " + std::to_string(m_port) +
                     "\n -> Root         : " + m_root +
                     "\n -> Thread       : " + std::to_string(m_thread));

  } catch (const boost::property_tree::ptree_error &e) {
    logger->error("HTTP_CONFIGURATION", "Configuration reading error");
    throw ConfigurationException("HTTP Configuration file is invalid");
  }
}

std::string ConfigurationServer::getDescription() const {
  return m_description;
};
std::string ConfigurationServer::getHostname() const { return m_hostname; };
std::string ConfigurationServer::getRoot() const { return m_root; };
uint8_t ConfigurationServer::getThreads() const { return m_thread; };
uint16_t ConfigurationServer::getPort() const { return m_port; };