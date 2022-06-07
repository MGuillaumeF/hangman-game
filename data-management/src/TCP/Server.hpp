#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

namespace hangman {
namespace tcp {
/**
 * @brief TCP Server class
 *
 */
class Server {
public:
  /**
   * @brief Construct a new TCP Server object
   *
   * @param ioContext connection context
   * @param port The connection port
   */
  Server(boost::asio::io_context &ioContext, const uint16_t& port);

private:
  /**
   * @brief accept connection
   *
   */
  void doAccept();

  boost::asio::ip::tcp::acceptor m_acceptor;
};

} // namespace tcp
} // namespace hangman
#endif // __TCP_SERVER_HPP__
