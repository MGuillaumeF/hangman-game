#ifndef __CONFIGURATION_SERVER_HPP__
#define __CONFIGURATION_SERVER_HPP__

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
  std::string getDescription() const;
  std::string getHostname() const;
  std::string getRoot() const;
  uint8_t getThreads() const;
  uint16_t getPort() const;

private:
  std::string m_description;
  std::string m_hostname;
  std::string m_root;
  unsigned short m_thread;
  unsigned m_port;
};

#endif