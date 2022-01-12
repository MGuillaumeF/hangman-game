#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include "Listener.hpp"

namespace http {
/**
 * Http Server Class is to run and stop HTTP Server
 */
class Server {
public:
  /**
   * @brief Construct a new Http Server:: Http Server object
   *
   * @param address The IP address of server
   * @param port The Port of server
   * @param threads The number of thread of server
   */
  Server(const std::string &address, const uint16_t port,
         const uint8_t threads);
};
} // namespace http

#endif
