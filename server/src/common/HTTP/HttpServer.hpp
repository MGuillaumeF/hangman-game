#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include "HttpListener.hpp"

/**
 * Http Server Class is to run and stop HTTP Server
 */
class HttpServer {
public:
  /**
   * HTTP Server definition and starting
   */
  HttpServer(const std::string &address, const uint16_t port,
             const std::string &doc_root, const uint8_t threads);
  ~HttpServer() {}
};

#endif