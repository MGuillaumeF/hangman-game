#include "./Session.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../connector/database.hxx"
#include "../endpoint/UserDBEndpoint.hpp"

namespace hangman {
namespace tcp {

Session::Session(boost::asio::ip::tcp::socket socket)
    : m_socket(std::move(socket)) {}

void Session::start() { doReadHead(); }

void Session::doRead() {
  const auto self(shared_from_this());
  m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                           [this, self](const boost::system::error_code &ec,
                                        const std::size_t &length) {
                             if (!ec) {
                               doWrite(length);
                             }
                           });
}

void Session::doReadHead() {
  const uint8_t SIZE_ALLOWED_BUFFER = 8;
  const auto self(shared_from_this());
  m_socket.async_read_some(boost::asio::buffer(m_data, SIZE_ALLOWED_BUFFER),
                           [this, self](const boost::system::error_code &ec,
                                        const std::size_t &length) {
                             if (!ec) {
                               const std::string messageSizeStr = m_data;
                               const uint32_t li_hex =
                                   std::stoul(messageSizeStr, nullptr, 16);
                               std::cout << "The header size is : " << std::dec
                                         << li_hex << std::endl;
                               doReadBody(li_hex);
                             }
                           });
}

void Session::doReadBody(const uint32_t &max_content) {
  const auto self(shared_from_this());
  m_socket.async_read_some(
      boost::asio::buffer(m_data, max_content),
      [this, self, max_content](const boost::system::error_code &ec,
                                const std::size_t &length) {
        if (!ec) {
          // Create an empty property tree object
          boost::property_tree::ptree xmlPtree;

          // Read the XML config string into the property tree. Catch any
          // exception
          try {
            std::ofstream currentFile("./currentOrder.xml");
            std::string content = m_data;
            content.resize(max_content);
            currentFile << content;
            currentFile.close();
            boost::property_tree::xml_parser::read_xml("./currentOrder.xml",
                                                       xmlPtree);
            std::cout << "OBJECT TYPE : "
                      << xmlPtree.get<std::string>(
                             "order.properties.object-type")
                      << std::endl;
            std::cout << "ORDER TYPE : "
                      << xmlPtree.get<std::string>(
                             "order.properties.order-type")
                      << std::endl;
            if (xmlPtree.get<std::string>("order.properties.object-type") ==
                "user") {

              // Create an empty property tree object
              UserDBEndpoint::getInstance()->createUser(
                  xmlPtree.get_child("order.user"));
            }

          } catch (
              const boost::property_tree::xml_parser::xml_parser_error &e) {
            std::cerr << "Failed to read received xml " << e.what()
                      << std::endl;
          } catch (const std::exception &ee) {
            std::cerr << "Failed to read received xml2 " << ee.what()
                      << std::endl;
          }
          std::cout << "The body is : " << m_data << std::endl;
          doWrite(length);
        }
      });
}

void Session::doWrite(const std::size_t &length) {
  const auto self(shared_from_this());
  boost::asio::async_write(m_socket, boost::asio::buffer(m_data, length),
                           [this, self](const boost::system::error_code &ec,
                                        const std::size_t /*length*/) {
                             if (!ec) {
                               // doReadHead();
                             }
                           });
}
} // namespace tcp
} // namespace hangman
