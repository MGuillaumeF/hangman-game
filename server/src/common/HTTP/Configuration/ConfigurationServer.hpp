#ifndef __CONFIGURATION_SERVER_HPP__
#define __CONFIGURATION_SERVER_HPP__

#include <iostream>
#include <vector>
#include <string>

/**
 * Configuration HTTP Server Class is to manage configuration of server
 */
class ConfigurationServer {
public:
  /**
   * @brief Construct a new Configuration Server object
   *
   * @param argv arguments list of server configuration
   */
  explicit ConfigurationServer(const std::vector<std::string>& argv);
  /**
   * @brief Destroy the Configuration Server object
   *
   */
  ~ConfigurationServer() = default;
  std::string getDescription() const;
  /**
   * @brief Get the Hostname object
   *
   * @return std::string
   */
  std::string getHostname() const;
  /**
   * @brief Get the Root object
   *
   * @return std::string
   */
  std::string getRoot() const;
  /**
   * @brief Get the Threads object
   *
   * @return uint8_t
   */
  uint8_t getThreads() const;
  /**
   * @brief Get the Port object
   *
   * @return uint16_t
   */
  uint16_t getPort() const;

private:
  std::string m_description;
  std::string m_hostname;
  std::string m_root;
  uint8_t m_thread;
  uint16_t  m_port;
};

#endif
