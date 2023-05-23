#include "HttpTokenEndpoint.hpp"

#include <boost/property_tree/ptree.hpp>

#include "../common/HTTP/Exception/ParsingException.hpp"
#include "../common/HTTP/Utils.hpp"
#include "../common/Logger/Logger.hpp"

/**
 * @brief Construct a new Http Token Endpoint:: Http Token Endpoint object
 *
 * @param req The request to handle
 */
HttpTokenEndpoint::HttpTokenEndpoint(
    const boost::beast::http::request<boost::beast::http::string_body> &req)
    : http::RestrictiveEndpoint(req,
                                // Only POST and DELETE method are allowed
                                {boost::beast::http::verb::post,
                                 boost::beast::http::verb::delete_}) {}

/**
 * @brief Method to overwrite POST method
 *
 */
void HttpTokenEndpoint::doPost() {
  // get HTTP request
  const boost::beast::http::request<boost::beast::http::string_body> request =
      this->getRequest();

  // get ptree of body by content-type parsing
  const boost::property_tree::ptree requestBodyTree =
      http::Utils::getBodyTree(request);

  // READ property tree content
  try {
    const std::string l_login = requestBodyTree.get<std::string>("login");
    const std::string l_pwd = requestBodyTree.get<std::string>("password");
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
    // ERROR TYPE : ENOKEY, mandatory key not found
    // FIELD : [NAME, ERROR]
    m_logger->error("HTTP_DATA_READ",
                    "HttpTokenEndpoint - doPost - body has at least one "
                    "mandatory field : " +
                        std::string(ex.what()));
    throw ParsingException("body has at least one mandatory field");
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_data> &) {
    // ERROR TYPE : EVALUETYPE, key found with bad value type
    // FIELD : [NAME, ERROR]
    throw ParsingException("body has bad value type");
  }

  // create http response of token
  boost::beast::http::response<boost::beast::http::string_body> res{
      boost::beast::http::status::ok, request.version()};
  res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(boost::beast::http::field::content_type, "text/plain");
  res.keep_alive(request.keep_alive());
  res.body() = "This is a fake token";
  res.prepare_payload();
  // set generated response
  setResponse(res);
} 
