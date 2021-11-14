// #define BOOST_TEST_DYN_LINK
// #define BOOST_TEST_MODULE testsTokenHttpAPI
// #include <boost/test/included/unit_test.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
// #include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <string>

struct REQUEST {
  std::string hostname = "localhost";
  std::string target = "/";
  std::map<std::string, std::string> headers;
  uint16_t port = 8000;
  uint8_t version = 11;
  std::string body = "";
  boost::beast::http::verb methode = boost::beast::http::verb::post;
};

boost::beast::http::response<boost::beast::http::dynamic_body>
sendRequest(REQUEST requestProperties) {
  // The io_context is required for all I/O
  boost::asio::io_context ioc;

  // These objects perform our I/O
  boost::asio::ip::tcp::resolver resolver(ioc);
  boost::beast::tcp_stream stream(ioc);

  // Look up the domain name
  auto const results =
      resolver.resolve(requestProperties.hostname.c_str(),
                       std::to_string(requestProperties.port).c_str());

  // Make the connection on the IP address we get from a lookup
  stream.connect(results);

  // Set up an HTTP GET request message
  boost::beast::http::request<boost::beast::http::string_body> request{
      requestProperties.methode, requestProperties.target,
      requestProperties.version};
  request.set(boost::beast::http::field::host,
              requestProperties.hostname.c_str());
  request.set(boost::beast::http::field::user_agent,
              BOOST_BEAST_VERSION_STRING);
  request.set(boost::beast::http::field::content_type, requestProperties.headers.at("Content-Type"));
  request.body() = requestProperties.body.c_str();
  request.prepare_payload();

  // Send the HTTP request to the remote host
  boost::beast::http::write(stream, request);

  // This buffer is used for reading and must be persisted
  boost::beast::flat_buffer buffer;

  // Declare a container to hold the response
  boost::beast::http::response<boost::beast::http::dynamic_body> res;

  // Receive the HTTP response
  boost::beast::http::read(stream, buffer, res);

  // Write the message to standard out
  std::cout << res << std::endl;

  // Gracefully close the socket
  boost::beast::error_code ec;
  stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

  // not_connected happens sometimes
  // so don't bother reporting it.
  //
  if (ec && ec != boost::beast::errc::not_connected) {

    throw boost::beast::system_error{ec};
  }
  return res;
};

// CREATE DIRECTORY RESOURCES OF REQUEST DEFINITIONS (XML FILES)
// CREATE STRUCTURE OF REQUEST DEFINITIONS FILES
// CREATE FUNCTION LOOP READ RESOURCES DIRECTORY
// CREATE FUNCTION TO EMIT REQUEST WITH BODY AND METHOD
// ADD ASSERT ON RESULT EXPECTED

int32_t main(int argc, char *argv[]) {
  uint8_t nbSuccess = 0;
  uint8_t nbFailed = 0;
  try {

    REQUEST requestProperties;
    requestProperties.port = 8080;
    requestProperties.target = "/api/token";
    requestProperties.headers = {{"Content-Type", "application/json"},
                                 {"Accept", "application/json"}};

    requestProperties.methode = boost::beast::http::verb::post;
    requestProperties.body = "{\"login\" : \"a\", \"password\" : \"b\"}";
    boost::beast::http::response<boost::beast::http::dynamic_body> response =
        sendRequest(requestProperties);

    if (response.result() == boost::beast::http::status::ok) {
      std::cout << "1 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "1 : [failed]" << std::endl;
      nbFailed++;
    }

    requestProperties.body = "{\"login\" : \"a\", \"errorKey\" : \"b\"}";
    response = sendRequest(requestProperties);
    if (response.result() == boost::beast::http::status::bad_request) {
      std::cout << "2 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "2 : [failed]" << std::endl;
      nbFailed++;
    }

    requestProperties.body = "'{\"login\" : \"a\", \"errorKey\" : \"b\"}'";
    response = sendRequest(requestProperties);
    if (response.result() == boost::beast::http::status::bad_request) {
      std::cout << "3 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "3 : [failed]" << std::endl;
      nbFailed++;
    }

    requestProperties.headers = {{"Content-Type", "application/xml"},
                                 {"Accept", "application/xml"}};

    response = sendRequest(requestProperties);
    if (response.result() == boost::beast::http::status::bad_request) {
      std::cout << "3 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "3 : [failed]" << std::endl;
      nbFailed++;
    }

    requestProperties.headers = {{"Content-Type", "text/html"},
                                 {"Accept", "text/html"}};

    response = sendRequest(requestProperties);
    if (response.result() == boost::beast::http::status::bad_request) {
      std::cout << "3 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "3 : [failed]" << std::endl;
      nbFailed++;
    }

    requestProperties.methode = boost::beast::http::verb::get;
    response = sendRequest(requestProperties);
    if (response.result() == boost::beast::http::status::method_not_allowed) {
      std::cout << "3 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "3 : [failed]" << std::endl;
      nbFailed++;
    }

    
    requestProperties.methode = boost::beast::http::verb::put;
    response = sendRequest(requestProperties);
    if (response.result() == boost::beast::http::status::method_not_allowed) {
      std::cout << "4 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "4 : [failed]" << std::endl;
      nbFailed++;
    }

    requestProperties.methode = boost::beast::http::verb::patch;
    response = sendRequest(requestProperties);
    if (response.result() == boost::beast::http::status::method_not_allowed) {
      std::cout << "5 : [success]" << std::endl;
      nbSuccess++;
    } else {
      std::cerr << "5 : [failed]" << std::endl;
      nbFailed++;
    }

    std::cout << std::endl
              << "Report tests" << std::endl
              << std::to_string(nbSuccess) << " tests passing" << std::endl
              << std::to_string(nbFailed) << "tests failed" << std::endl;
  } catch (const std::exception &ec) {

    std::cerr << "[ERROR] " << ec.what() << std::endl;
  }
  return nbFailed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
