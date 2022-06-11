#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <memory>   // unique_ptr
#include <thread>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream
#include <string>  // std::string

#include <boost/asio.hpp>

int32_t main(int argc, char *argv[]) {

  try {
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::socket s(io_context);
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("localhost", "50000"));

    std::ifstream createUserFile("./resources/database-order/create-user.xml");
    std::stringstream createUserFileStream;
    createUserFileStream << createUserFile.rdbuf();

    std::cout << "Enter message: " << std::endl;
    const std::string request = createUserFileStream.str();

    std::cout << request << std::endl;
    std::cout << std::hex << request << std::endl;
    size_t request_length = request.size();

    std::cout << std::setfill('0') << std::setw(8);
    std::cout << std::hex << request_length << std::endl;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(8);
    ss << std::hex << request_length;
    ss << std::hex << request;
    boost::asio::write(
        s, boost::asio::buffer(ss.str().c_str(), request_length + 8));

    char reply[1024];
    size_t reply_length =
        boost::asio::read(s, boost::asio::buffer(reply, request_length));
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}