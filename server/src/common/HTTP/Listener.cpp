#include "Listener.hpp"

#include "../Logger/Logger.hpp"
#include "Session.hpp"
#include "Utils.hpp"

namespace http {
/**
 * @brief Construct a new Http Listener:: Http Listener object
 *
 * @param ioc The context to listen
 * @param endpoint The TCP/IP endpoint
 * @param doc_root The root path of file server
 */
Listener::Listener(boost::asio::io_context &ioc,
                   const boost::asio::ip::tcp::endpoint &endpoint,
                   std::shared_ptr<std::string const> const &doc_root)
    : m_ioc(ioc), m_acceptor(boost::asio::make_strand(ioc)),
      m_doc_root(doc_root) {

  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  boost::beast::error_code ec;

  // Open the acceptor
  m_acceptor.open(endpoint.protocol(), ec);
  if (ec) {
    logger->error("HTTP_CONFIGURATION",
                  "Acceptor opening failed " + ec.message());
  } else {
    // Allow address reuse
    m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
      logger->error("HTTP_CONFIGURATION",
                    "Acceptor option settings failed " + ec.message());
    } else {
      // Bind to the server address
      m_acceptor.bind(endpoint, ec);
      if (ec) {
        logger->error("HTTP_CONFIGURATION",
                      "Acceptor binding failed " + ec.message());
      } else {
        // Start listening for connections
        m_acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
        if (ec) {
          logger->error("HTTP_CONFIGURATION",
                        "Acceptor listening failed " + ec.message());
        } else {
          logger->info("HTTP_CONFIGURATION", "Acceptor listening");
        }
      }
    }
  }
}

/**
 * Start accepting incoming connections
 */
void Listener::run() { doAccept(); }

/**
 * Dispatch new connection to gets its own strand
 */
void Listener::doAccept() {
  // The new connection gets its own strand
  m_acceptor.async_accept(boost::asio::make_strand(m_ioc),
                          boost::beast::bind_front_handler(&Listener::onAccept,
                                                           shared_from_this()));
}
/**
 * @brief Create the session for the new connection and run it
 *
 * @param ec The error code of previous step
 * @param socket The TCP/IP socket
 */
void Listener::onAccept(const boost::beast::error_code &ec,
                        boost::asio::ip::tcp::socket socket) {
  const std::unique_ptr<Logger> &logger = Logger::getInstance();
  if (ec) {
    logger->error("HTTP_CONFIGURATION", "onAccept error " + ec.message());
  } else {
    // Create the session and run it
    std::make_shared<HTTP::Session>(std::move(socket), m_doc_root)->run();
  }

  // Accept another connection
  doAccept();
}
} // namespace HTTP
