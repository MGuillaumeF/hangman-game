#include "./Server.hpp"
#include "./Session.hpp"

namespace hangman {
namespace tcp {

/**
 * @brief Construct a new TCP Server object
 *
 * @param ioContext The input/output context to use to run TCP server
 * @param port The port to use to listen connection and content
 */
Server::Server(boost::asio::io_context &ioContext, const uint16_t &port)
    : m_acceptor(ioContext, boost::asio::ip::tcp::endpoint(
                                boost::asio::ip::tcp::v4(), port)) {
  // add log server is started and listen at port xxxx
  std::cout << "Server listen at " << port << std::endl;
  // accept connection and start session
  doAccept();
}
/**
 * @brief accept connection
 *
 */
void Server::doAccept() {
  // run accept as async function
  m_acceptor.async_accept([this](const boost::system::error_code &ec,
                                 boost::asio::ip::tcp::socket socket) {
    // if accept success without error start session
    if (!ec) {
      // start session with shared pointer
      std::make_shared<Session>(std::move(socket))->start();
      // trace Client is connected
      std::cout << "Client connection" << std::endl;
    } else {
      std::cerr << "[ERROR] Client connection denied with error " << ec.value() << std::endl;
    }
    // loop accept another content
    doAccept();
  });
}
} // namespace tcp
} // namespace hangman
