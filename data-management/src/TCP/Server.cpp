#include "./Server.hpp"
#include "./Session.hpp"

namespace hangman {
namespace tcp {

/**
 * @brief TCP Server class
 *
 */
Server::Server(boost::asio::io_context &ioContext, uint16_t port)
    : m_acceptor(ioContext, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port)) {
  doAccept();
}
/**
 * @brief accept connection
 *
 */
void Server::doAccept() {
  m_acceptor.async_accept([this](boost::system::error_code ec,
                                 boost::asio::ip::tcp::socket socket) {
    if (!ec) {
      std::make_shared<Session>(std::move(socket))->start();
    }

    doAccept();
  });
}
} // namespace tcp
} // namespace hangman