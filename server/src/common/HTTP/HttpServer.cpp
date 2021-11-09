#include "HttpServer.hpp"
#include "../Logger/Logger.hpp"

#include <thread>

/**
 * @brief Construct a new Http Server:: Http Server object
 *
 * @param ioc The IO Context of application
 * @param address The IP address of server
 * @param port The Port of server
 * @param doc_root The path of root directory of static files
 * @param threads The number of thread of server
 */
HttpServer::HttpServer(boost::asio::io_context& ioc, const std::string &address, const uint16_t port,
                       const std::string &doc_root, const uint8_t threads) {
  auto const l_address = boost::asio::ip::make_address(address);
  auto const l_doc_root = std::make_shared<std::string>(doc_root);

  // Create and launch a listening port
  std::make_shared<HttpListener>(
      ioc, boost::asio::ip::tcp::endpoint{l_address, port}, l_doc_root)
      ->run();

  // Run the I/O service on the requested number of threads
  std::vector<std::thread> threadList;
  threadList.reserve(threads);
  for (auto i = 0; i < threads; i++) {
    threadList.emplace_back([&ioc] { ioc.run(); });
  }
}
