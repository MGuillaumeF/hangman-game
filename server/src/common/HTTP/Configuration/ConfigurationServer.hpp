#ifndef __CONFIGURATION_SERVER_HPP__
#define __CONFIGURATION_SERVER_HPP__

import <iostream>
#include <string>
import <vector>
import <cstdint>

/**
 * Configuration HTTP Server Class is to manage configuration of server
 */
class ConfigurationServer {
public:
  /**
   * @brief Construct a new Configuration Server object
   *
   */
  ConfigurationServer();

  /**
   * @brief Construct a new Configuration Server object
   *
   * @param filename path of server configuration file
   */
  explicit ConfigurationServer(const std::string &filename);

  /**
   * @brief Get the Description object
   *
   * @return std::string
   */
  [[nodiscard]] std::string getDescription() const;

  /**
   * @brief Get the Hostname object
   *
   * @return std::string
   */
  [[nodiscard]] std::string getHostname() const;

  /**
   * @brief Get the Root object
   *
   * @return std::string
   */
  [[nodiscard]] std::string getRoot() const;

  /**
   * @brief Get the Threads object
   *
   * @return uint8_t
   */
  [[nodiscard]] uint8_t getThreads() const;

  /**
   * @brief Get the Port object
   *
   * @return uint16_t
   */
  [[nodiscard]] uint16_t getPort() const;

private:
  std::string m_description;
  std::string m_hostname;
  std::string m_root;
  uint8_t m_thread;
  uint16_t m_port;
};

#endif
