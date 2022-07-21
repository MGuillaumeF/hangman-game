#include "Server.hpp"
#include "../Logger/Logger.hpp"
// import to subscribe system signals like stop/kill process
#include <boost/asio/signal_set.hpp>
// import for multi thread server
#include <thread>

namespace http {
/**
 * @brief Construct a new Http Server:: Http Server object
 *
 * @param address The IP address of server
 * @param port The Port of server
 * @param threads The number of thread of server
 */
Server::Server(const std::string &address, const uint16_t &port,
               const uint8_t &threads) {
  // get add address from string
  auto const l_address = boost::asio::ip::make_address(address);

  // The io_context is required for all I/O
  boost::asio::io_context ioc{threads};

  // Create and launch a listening port
  std::make_shared<Listener>(ioc,
                             boost::asio::ip::tcp::endpoint{l_address, port})
      ->run();

  // Run the I/O service on the requested number of threads
  std::vector<std::thread> threadList;
  // reserve place for each thread to start
  threadList.reserve(threads);
  for (auto i = 0; i < threads; i++) {
    // start server for each thread
    threadList.emplace_back([&ioc] { ioc.run(); });
  }
  // create signal on close process order
  boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
  // subscription on signal action to soft stop listener context
  signals.async_wait(
      [&ioc](const boost::system::error_code &ec, const int32_t &n) {
        // log stop message 
        Logger::getInstance()->info(
            "HTTP_CONFIGURATION",
            std::string("IO Context stop with ") + ec.message() +
                std::string(" and handler code : ") + std::to_string(n));
        // stop listener context
        ioc.stop();
        // exit process with success state
        std::exit(EXIT_SUCCESS);
      });
  // run server listeners on context
  ioc.run();
}
} // namespace http
