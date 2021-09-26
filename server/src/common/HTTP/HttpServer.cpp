#include "HttpServer.hpp"
#include <thread>

HttpServer::HttpServer(char *address, char *port, char *doc_root,
                       char *threads) {
  auto const l_address = boost::asio::ip::make_address(address);
  auto const l_port = static_cast<unsigned short>(std::atoi(port));
  auto const l_doc_root = std::make_shared<std::string>(doc_root);
  auto const l_threads = std::max<int>(1, std::atoi(threads));

  // The io_context is required for all I/O
  boost::asio::io_context ioc{l_threads};

  // Create and launch a listening port
  std::make_shared<HttpListener>(
      ioc, boost::asio::ip::tcp::endpoint{l_address, l_port}, l_doc_root)
      ->run();

  // Run the I/O service on the requested number of threads
  std::vector<std::thread> v;
  v.reserve(l_threads - 1);
  for (auto i = l_threads - 1; i > 0; --i) {
    v.emplace_back([&ioc] { ioc.run(); });
  }
  boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait(boost::bind(&boost::asio::io_context::stop, &ioc));
  ioc.run();
}
