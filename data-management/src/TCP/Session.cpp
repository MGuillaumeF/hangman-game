#include "./Session.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../connector/database.hxx"
#include "../endpoint/GroupeOrderDispatcher.hpp"

namespace hangman {
namespace tcp {

Session::Session(boost::asio::ip::tcp::socket socket)
    : m_socket(std::move(socket)) {}

void Session::start() { doReadHead(); }

void Session::doReadHead() {
  const uint8_t SIZE_ALLOWED_BUFFER = 8;
  const auto self(shared_from_this());
  m_socket.async_read_some(
      boost::asio::buffer(m_data, SIZE_ALLOWED_BUFFER),
      [this, self](const boost::system::error_code &ec,
                   const std::size_t &length) {
        if (!ec) {
          const std::string messageSizeStr = m_data;
          const uint32_t li_hex = std::stoul(messageSizeStr, nullptr, 16);
          std::cout << "The header size is : " << std::dec << li_hex
                    << std::endl;
          doReadBody(li_hex);
        } else {
          // add log of error code during header reading tcp request
          std::cerr
              << "Error : TCP header reading raise error << ec.value() << std::endl;
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
          const uint8_t SIZE_ALLOWED_BUFFER = 8;
          std::stringstream responseStr;
          // Create an empty property tree object
          boost::property_tree::ptree xmlPtree;

          std::stringstream currentRequest;
          std::string content = m_data;
          content.resize(max_content);
          currentRequest << content;
          // Read the XML config string into the property tree. Catch any
          // exception
          try {
            boost::property_tree::xml_parser::read_xml(currentRequest,
                                                       xmlPtree);
            const boost::property_tree::ptree response =
                GroupeOrderDispatcher::route(
                    xmlPtree.get<std::string>("order.properties.order-group"),
                    xmlPtree.get_child("order.properties"),
                    xmlPtree.get_child("order.data"));

            boost::property_tree::xml_parser::write_xml(responseStr, response);

          } catch (
              const boost::property_tree::xml_parser::xml_parser_error &e) {
            std::cerr << "Failed to read received xml " << e.what()
                      << std::endl;
          } catch (const std::exception &ee) {
            std::cerr << "Failed to read received xml2 " << ee.what()
                      << std::endl;
          }
          std::cout << "The body is of request : " << content << std::endl;
          std::cout << "The body is of response : " << responseStr.str()
                    << std::endl;

          // format response
          std::stringstream stream;
          stream << std::setfill('0') << std::setw(SIZE_ALLOWED_BUFFER);
          stream << std::hex << responseStr.str().size();
          stream << std::hex << responseStr.str();

          doWrite(stream.str());
        } else {
          // add log of error code during body reading tcp request
          std::cerr
              << "Error : TCP body reading raise error << ec.value() << std::endl;
        }
      });
}

void Session::doWrite(const std::string &response) {
  const auto self(shared_from_this());
  boost::asio::async_write(
      m_socket, boost::asio::buffer(response.c_str(), response.size()),
      [self](const boost::system::error_code &ec, const std::size_t) {
        if (ec) {
          // add log of error code during write tcp response
          std::cerr
              << "Error : TCP write raise error << ec.value() << std::endl;
        }
      });
}
} // namespace tcp
} // namespace hangman
