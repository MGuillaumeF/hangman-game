
#include "HttpSession.hpp"
#include "../../api/HttpFruitsEndpoint.hpp"
#include "../../api/HttpTokenEndpoint.hpp"

/**
 * Append an HTTP rel-path to a local filesystem path.
 * The returned path is normalized for the platform.
 */
std::string HttpSession::pathCat(boost::beast::string_view base,
                                 boost::beast::string_view path) {
  if (base.empty())
    return std::string(path);
  std::string result(base);
#ifdef BOOST_MSVC
  char constexpr path_separator = '\\';
  if (result.back() == path_separator)
    result.resize(result.size() - 1);
  result.append(path.data(), path.size());
  for (auto &c : result)
    if (c == '/')
      c = path_separator;
#else
  char constexpr path_separator = '/';
  if (result.back() == path_separator)
    result.resize(result.size() - 1);
  result.append(path.data(), path.size());
#endif
  return result;
}

/** This function produces an HTTP response for the given
 * request. The type of the response object depends on the
 * contents of the request, so the interface requires the
 * caller to pass a generic lambda for receiving the response.
 */
template <class Body, class Allocator, class Send>
void HttpSession::handleRequest(
    boost::beast::string_view doc_root,
    boost::beast::http::request<
        Body, boost::beast::http::basic_fields<Allocator>> &&req,
    Send &&send) {

  // Returns a bad request response
  auto const bad_request = [&req](boost::beast::string_view why) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::bad_request, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();
    return res;
  };

  // Returns a not found response
  auto const not_found = [&req](boost::beast::string_view target) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::not_found, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "The resource '" + std::string(target) + "' was not found.";
    res.prepare_payload();
    return res;
  };

  // Returns a server error response
  auto const server_error = [&req](boost::beast::string_view what) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::internal_server_error, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "An error occurred: '" + std::string(what) + "'";
    res.prepare_payload();
    return res;
  };

  auto const buildResponse =
      [](boost::beast::http::response<boost::beast::http::string_body> res) {
        return res;
      };
  try {
    if (req.target().compare("/api/token") == 0) {
      std::cout << "handleRequest - /api/token" << std::endl;
      HttpTokenEndpoint tokenEndpoint(req);
      tokenEndpoint.dispatchRequest();
      boost::beast::http::response<boost::beast::http::string_body> response =
          tokenEndpoint.getResponse();
      return send(buildResponse(response));
    }

    if (req.target().compare("/api/fruits") == 0) {
      std::cout << "handleRequest - /api/fruits" << std::endl;
      HttpFruitsEndpoint fruits(req);
      fruits.dispatchRequest();
      boost::beast::http::response<boost::beast::http::string_body> response =
          fruits.getResponse();
      return send(buildResponse(response));
    }
  } catch (const ParsingException &ex) {
    std::cerr << "handleRequest - parsing error : " << ex.what() << std::endl;

    return send(bad_request("parsing error"));
  }
  // Make sure we can handle the method
  if (req.method() != boost::beast::http::verb::get &&
      req.method() != boost::beast::http::verb::head)
    return send(bad_request("Unknown HTTP-method"));

  // Request path must be absolute and not contain "..".
  if (req.target().empty() || req.target()[0] != '/' ||
      req.target().find("..") != boost::beast::string_view::npos)
    return send(bad_request("Illegal request-target"));

  // Build the path to the requested file
  std::string path = pathCat(doc_root, req.target());
  if (req.target().back() == '/')
    path.append("index.html");

  // Attempt to open the file
  boost::beast::error_code ec;
  boost::beast::http::file_body::value_type body;
  body.open(path.c_str(), boost::beast::file_mode::scan, ec);

  // Handle the case where the file doesn't exist
  if (ec == boost::beast::errc::no_such_file_or_directory)
    return send(not_found(req.target()));

  // Handle an unknown error
  if (ec)
    return send(server_error(ec.message()));

  // Cache the size since we need it after the move
  auto const size = body.size();

  // Respond to HEAD request
  if (req.method() == boost::beast::http::verb::head) {
    boost::beast::http::response<boost::beast::http::empty_body> res{
        boost::beast::http::status::ok, req.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type,
            HttpUtils::getMimeType(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
  }

  // Respond to GET request
  boost::beast::http::response<boost::beast::http::file_body> res{
      std::piecewise_construct, std::make_tuple(std::move(body)),
      std::make_tuple(boost::beast::http::status::ok, req.version())};
  res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(boost::beast::http::field::content_type,
          HttpUtils::getMimeType(path));
  res.content_length(size);
  res.keep_alive(req.keep_alive());
  return send(std::move(res));
}

// Start the asynchronous operation
void HttpSession::run() {
  // We need to be executing within a strand to perform async operations
  // on the I/O objects in this session. Although not strictly necessary
  // for single-threaded contexts, this example code is written to be
  // thread-safe by default.
  boost::asio::dispatch(m_stream.get_executor(),
                        boost::beast::bind_front_handler(&HttpSession::doRead,
                                                         shared_from_this()));
}

void HttpSession::doRead() {
  // Make the request empty before reading,
  // otherwise the operation behavior is undefined.
  m_req = {};

  // Set the timeout.
  m_stream.expires_after(std::chrono::seconds(30));

  // Read a request
  boost::beast::http::async_read(m_stream, m_buffer, m_req,
                                 boost::beast::bind_front_handler(
                                     &HttpSession::onRead, shared_from_this()));
}

void HttpSession::onRead(boost::beast::error_code ec,
                         std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // This means they closed the connection
  if (ec == boost::beast::http::error::end_of_stream)
    return doClose();

  if (ec)
    return HttpUtils::onFail(ec, "read");

  // Send the response
  handleRequest(*m_doc_root, std::move(m_req), m_lambda);
}

void HttpSession::onWrite(bool close, boost::beast::error_code ec,
                          std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec)
    return HttpUtils::onFail(ec, "write");

  if (close) {
    // This means we should close the connection, usually because
    // the response indicated the "Connection: close" semantic.
    return doClose();
  }

  // We're done with the response so delete it
  m_res = nullptr;

  // Read another request
  doRead();
}

void HttpSession::doClose() {
  // Send a TCP shutdown
  boost::beast::error_code ec;
  m_stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);

  // At this point the connection is closed gracefully
}