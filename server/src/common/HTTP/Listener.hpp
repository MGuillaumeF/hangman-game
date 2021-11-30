#ifndef __HTTP_LISTENER_HPP__
#define __HTTP_LISTENER_HPP__

#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>

namespace http {
/**
 *  Accepts incoming connections and launches the sessions
 */
class Listener : public std::enable_shared_from_this<Listener> {
  boost::asio::io_context &m_ioc;
  boost::asio::ip::tcp::acceptor m_acceptor;

public:
  /**
   * @brief Construct a new Http Listener:: Http Listener object
   *
   * @param ioc The context to listen
   * @param endpoint The TCP/IP endpoint
   * @param doc_root The root path of file server
   */
  Listener(boost::asio::io_context &ioc,
           const boost::asio::ip::tcp::endpoint &endpoint);

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
   * @brief Create the session for the new connection and run it
   *
   * @param ec The error code of previous step
   * @param socket The TCP/IP socket
   */
  void onAccept(const boost::beast::error_code &ec,
                boost::asio::ip::tcp::socket socket);
};
} // namespace http
#endif
