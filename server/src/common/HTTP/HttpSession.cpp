
#include "HttpSession.hpp"

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
    http::request<Body, http::basic_fields<Allocator>> &&req, Send &&send) {

  // Returns a bad request response
  auto const printResponse = [&req]() {
    http::response<http::string_body> res{http::status::bad_request,
                                          req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());

    // Create a root
    pt::ptree root;

    root.put("height", 320);
    root.put("some.complex.path", "bonjour");

    // Create a node
    pt::ptree animals_node;
    std::string animalsNames[3] = {"rabbit", "dog", "cat"};
    std::string animalsColors[3] = {"white", "brown", "grey"};

    // Add animals as childs
    for (int i = 0; i < 3; i += 1)
      animals_node.put(animalsNames[i], animalsColors[i]);
    // Add the new node to the root
    root.add_child("animals", animals_node);

    // Add two objects with the same name
    pt::ptree fish1;
    fish1.put_value("blue");
    pt::ptree fish2;
    fish2.put_value("yellow");
    root.push_back(std::make_pair("fish", fish1));
    root.push_back(std::make_pair("fish", fish2));

    std::string fruits[3] = {"banana", "strawsberry", "orange"};
    // Add a list
    pt::ptree fruits_node;
    for (auto &fruit : fruits) {
      // Create an unnamed node containing the value
      pt::ptree fruit_node;
      fruit_node.put("", fruit);

      // Add this node to the list.
      fruits_node.push_back(std::make_pair("", fruit_node));
    }
    root.add_child("fruits", fruits_node);

    std::ostringstream buf;
    pt::write_json(buf, root, false);

    res.body() = buf.str();
    res.prepare_payload();
    return res;
  };

  // Returns a bad request response
  auto const bad_request = [&req](boost::beast::string_view why) {
    http::response<http::string_body> res{http::status::bad_request,
                                          req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();
    return res;
  };

  // Returns a not found response
  auto const not_found = [&req](boost::beast::string_view target) {
    http::response<http::string_body> res{http::status::not_found,
                                          req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "The resource '" + std::string(target) + "' was not found.";
    res.prepare_payload();
    return res;
  };

  // Returns a server error response
  auto const server_error = [&req](boost::beast::string_view what) {
    http::response<http::string_body> res{http::status::internal_server_error,
                                          req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "An error occurred: '" + std::string(what) + "'";
    res.prepare_payload();
    return res;
  };

  // Make sure we can handle the method
  if (req.method() != http::verb::get && req.method() != http::verb::head)
    return send(bad_request("Unknown HTTP-method"));

  if (req.target() == "/api/print") {
    return send(printResponse());
  }

  if (req.target() == "/api/fruits") {
    std::map<boost::beast::http::verb, bool> allowedMethods = {
        {boost::beast::http::verb::post, false},
        {boost::beast::http::verb::get, true},
        {boost::beast::http::verb::put, false},
        {boost::beast::http::verb::patch, false},
        {boost::beast::http::verb::delete_, false}};
    // HttpRestrictiveEndpoint fruits(req, send, allowedMethods);
  }

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
  http::file_body::value_type body;
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
  if (req.method() == http::verb::head) {
    http::response<http::empty_body> res{http::status::ok, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, HttpUtils::getMimeType(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
  }

  // Respond to GET request
  http::response<http::file_body> res{
      std::piecewise_construct, std::make_tuple(std::move(body)),
      std::make_tuple(http::status::ok, req.version())};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, HttpUtils::getMimeType(path));
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
  http::async_read(m_stream, m_buffer, m_req,
                   boost::beast::bind_front_handler(&HttpSession::onRead,
                                                    shared_from_this()));
}

void HttpSession::onRead(boost::beast::error_code ec,
                         std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // This means they closed the connection
  if (ec == http::error::end_of_stream)
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