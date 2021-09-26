#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include "HttpListener.hpp"
#include <boost/asio/signal_set.hpp>

/**
 * Http Server Class is to run and stop HTTP Server
 */
class HttpServer {
public:
  /**
   * HTTP Server definition and starting
   */
  HttpServer(char *address, char *port, char *doc_root, char *threads);
  ~HttpServer() {}
};

#endif