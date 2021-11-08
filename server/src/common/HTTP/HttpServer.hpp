#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include "HttpListener.hpp"

#include <boost/asio/io_context.hpp>

/**
 * Http Server Class is to run and stop HTTP Server
 */
class HttpServer {
public:
  /**
   * @brief Construct a new Http Server:: Http Server object
   *
   * @param address The IP address of server
   * @param port The Port of server
   * @param doc_root The path of root directory of static files
   * @param threads The number of thread of server
   */
  HttpServer(boost::asio::io_context& ioc, const std::string &address, const uint16_t port,
             const std::string &doc_root, const uint8_t threads);
  /**
   * @brief Destroy the Http Server object
   *
   */
  ~HttpServer() = default;
};

#endif
