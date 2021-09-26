#ifndef __HTTP_LISTENER_H__
#define __HTTP_LISTENER_H__

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>

#include "HttpSession.hpp"
#include "HttpUtils.hpp"

/**
 *  Accepts incoming connections and launches the sessions
 */
class HttpListener : public std::enable_shared_from_this<HttpListener> {
  boost::asio::io_context &m_ioc;
  boost::asio::ip::tcp::acceptor m_acceptor;
  std::shared_ptr<std::string const> m_doc_root;

public:
  /**
   * Constructor of class
   */
  HttpListener(boost::asio::io_context &ioc,
               boost::asio::ip::tcp::endpoint endpoint,
               std::shared_ptr<std::string const> const &doc_root);

  /**
   * Start accepting incoming connections
   */
  void run();

private:
  /**
   * Dispatch new connection to gets its own strand
   */
  void doAccept();
  /**
   * Create the session for the new connection and run it
   */
  void onAccept(boost::beast::error_code ec,
                boost::asio::ip::tcp::socket socket);
};

#endif