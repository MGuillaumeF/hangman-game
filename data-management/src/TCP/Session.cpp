#include "./Session.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../connector/database.hxx"
#include "../endpoint/UserDBEndpoint.hpp"

/**
 * @brief Get the Data Base Access object
 *
 * @return std::unique_ptr<odb::core::database>
 */
std::unique_ptr<odb::core::database> getDataBaseAccess2() {
  char *tempArgv[] = {"_", "--user", "odb_test", "--database", "data2.db"};
  int32_t tempArgc = static_cast<int32_t>(sizeof(tempArgv));

  return create_database(tempArgc, tempArgv);
}

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
  const auto self(shared_from_this());
  m_socket.async_read_some(boost::asio::buffer(m_data, 8),
                           [this, self](const boost::system::error_code &ec,
                                        const std::size_t &length) {
                             if (!ec) {
                               const std::string messageSizeStr = m_data;
                               const uint32_t li_hex =
                                   std::stol(messageSizeStr, nullptr, 16);
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
            content.resize(max_content - 1);
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
            // if (xmlPtree.get<std::string>(
            //                    "order.properties.object-type") == "user") {

            // Create an empty property tree object
            // boost::property_tree::ptree userPtree;
            // const user newUser = user::parseXml(content);
            // UserDBEndpoint::createUser(getDataBaseAccess2(),
            // xmlPtree.get_child("order.user"));

            // }

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
