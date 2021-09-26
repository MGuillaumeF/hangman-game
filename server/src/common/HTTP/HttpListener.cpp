#include "HttpListener.hpp"

HttpListener::HttpListener(boost::asio::io_context &ioc,
                           boost::asio::ip::tcp::endpoint endpoint,
                           std::shared_ptr<std::string const> const &doc_root)
    : m_ioc(ioc), m_acceptor(boost::asio::make_strand(ioc)),
      m_doc_root(doc_root) {
  boost::beast::error_code ec;

  // Open the acceptor
  m_acceptor.open(endpoint.protocol(), ec);
  if (ec) {
    HttpUtils::onFail(ec, "open");
    return;
  }

  // Allow address reuse
  m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
  if (ec) {
    HttpUtils::onFail(ec, "set_option");
    return;
  }

  // Bind to the server address
  m_acceptor.bind(endpoint, ec);
  if (ec) {
    HttpUtils::onFail(ec, "bind");
    return;
  }

  // Start listening for connections
  m_acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
  if (ec) {
    HttpUtils::onFail(ec, "listen");
    return;
  }
}

// Start accepting incoming connections
void HttpListener::run() { doAccept(); }

void HttpListener::doAccept() {
  // The new connection gets its own strand
  m_acceptor.async_accept(boost::asio::make_strand(m_ioc),
                          boost::beast::bind_front_handler(
                              &HttpListener::onAccept, shared_from_this()));
}

void HttpListener::onAccept(boost::beast::error_code ec,
                            boost::asio::ip::tcp::socket socket) {
  if (ec) {
    HttpUtils::onFail(ec, "accept");
  } else {
    // Create the session and run it
    std::make_shared<HttpSession>(std::move(socket), m_doc_root)->run();
  }

  // Accept another connection
  doAccept();
}