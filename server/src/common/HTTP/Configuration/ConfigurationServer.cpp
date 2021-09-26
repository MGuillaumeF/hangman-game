#include "ConfigurationServer.hpp"

ConfigurationServer::ConfigurationServer(char *argv[]) {

  boost::property_tree::ptree pt;
  boost::property_tree::read_xml("../resources/configuration.xml", pt);

  try {
    m_description = pt.get<std::string>("server.description");
    m_hostname = pt.get<std::string>("server.hostname");
    m_root = pt.get<std::string>("server.root");
    m_thread = pt.get<unsigned short>("server.thread");
    m_port = pt.get<unsigned>("server.port");

    std::cerr << std::endl
              << "Configuration reading : " << std::endl
              << std::endl
              << " -> Description  : " << m_description << std::endl
              << " -> Hostname     : " << m_hostname << std::endl
              << " -> Port         : " << m_port << std::endl
              << " -> Root         : " << m_root << std::endl
              << " -> Thread       : " << m_thread << std::endl;

  } catch (const boost::property_tree::ptree_error &e) {
    std::cerr << "Configuration reading error : " << e.what() << std::endl;
    throw std::runtime_error("Configuration file is invalid");
  }
}