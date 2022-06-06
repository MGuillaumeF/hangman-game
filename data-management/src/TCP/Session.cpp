#include "./Session.hpp"
#include <iostream>
namespace hangman {
namespace tcp {

Session::Session(boost::asio::ip::tcp::socket socket)
    : m_socket(std::move(socket)) {}

void Session::start() { doReadHead(); }

void Session::doRead() {
  auto self(shared_from_this());
  m_socket.async_read_some(
      boost::asio::buffer(m_data, max_length),
      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          doWrite(length);
        }
      });
}

void Session::doReadHead() {
  auto self(shared_from_this());
  m_socket.async_read_some(
      boost::asio::buffer(m_data, 4),
      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          std::cout << "The head size is : " << m_data << std::endl;
          doReadBody(atoi(m_data));
        }
      });
}

void Session::doReadBody(uint32_t max_content) {
  auto self(shared_from_this());
  m_socket.async_read_some(
      boost::asio::buffer(m_data, max_content),
      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          std::cout << "The body is : " << m_data << std::endl;
          doWrite(length);
        }
      });
}

void Session::doWrite(std::size_t length) {
  auto self(shared_from_this());
  boost::asio::async_write(
      m_socket, boost::asio::buffer(m_data, length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          // doReadHead();
        }
      });
}
} // namespace tcp
} // namespace hangman