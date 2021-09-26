#ifndef __CONFIGURATION_SERVER_HPP__
#define __CONFIGURATION_SERVER_HPP__

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

/**
 * Configuration HTTP Server Class is to manage configuration of server
 */
class ConfigurationServer {
public:
  /**
   * Configuration Server definition
   */
  ConfigurationServer(char *argv[]);
  ~ConfigurationServer() {}

private:
  std::string m_description;
  std::string m_hostname;
  std::string m_root;
  // std::string m_logs_path;
  // std::string m_logs_level;
  unsigned short m_thread;
  unsigned m_port;
};

#endif