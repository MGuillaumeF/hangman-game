#ifndef __TCP_SESSION_HPP__
#define __TCP_SESSION_HPP__

#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

namespace hangman {
namespace tcp {

/**
 * @brief TCP session of exchange
 *
 */
class Session : public std::enable_shared_from_this<Session> {
public:
  explicit Session(boost::asio::ip::tcp::socket socket);

  void start();

private:
  void doReadHead();
  void doReadBody(const uint32_t &max_content);

  void doWrite(const std::string &response);

  boost::asio::ip::tcp::socket m_socket;
  enum { max_length = 1024 };
  char m_data[max_length];
};

} // namespace tcp
} // namespace hangman

#endif // __TCP_SERVER_HPP__
