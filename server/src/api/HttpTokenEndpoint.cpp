#include "HttpTokenEndpoint.hpp"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../common/HTTP/Exception/ParsingException.hpp"
#include "../common/Logger/Logger.hpp"

/**
 * @brief Construct a new Http Token Endpoint:: Http Token Endpoint object
 *
 * @param req The request to handle
 */
HttpTokenEndpoint::HttpTokenEndpoint(
    const boost::beast::http::request<boost::beast::http::string_body> &req)
    : http::RestrictiveEndpoint(req,
                                // Only POST methode is activated
                                {{boost::beast::http::verb::post, true},
                                 {boost::beast::http::verb::get, false},
                                 {boost::beast::http::verb::put, false},
                                 {boost::beast::http::verb::patch, false},
                                 {boost::beast::http::verb::delete_, true}}) {
  m_logger->setLevel(ELogLevel::LINFO);

  m_logger->debug("HTTP_DATA_READ", "HttpTokenEndpoint - constructor");
}

/**
 * @brief Methode to overwrite POST methode
 *
 */
void HttpTokenEndpoint::doPost() {
  const boost::beast::http::request<boost::beast::http::string_body> request =
      this->getRequest();
  m_logger->debug("HTTP_DATA_READ", "HttpTokenEndpoint - doPost - start");

  std::stringstream l_stream(request.body());
  boost::property_tree::ptree requestBodyTree;

  if (const boost::string_view contentType =
          request.at(boost::beast::http::field::content_type);
      0 == contentType.compare("application/json")) {
    m_logger->debug("HTTP_DATA_READ",
                    "HttpTokenEndpoint - doPost - json body content expected");
    try {
      // read JSON request body in string stream
      boost::property_tree::read_json(l_stream, requestBodyTree);
    } catch (const std::exception &ex) {
      // request body tree is invalid
      m_logger->error(
          "HTTP_DATA_READ",
          "HttpTokenEndpoint - doPost - JSON body has invalid structure" +
              std::string(ex.what()));
      throw ParsingException("body has invalid structure");
    }
  } else if (0 == contentType.compare("application/xml")) {
    m_logger->debug("HTTP_DATA_READ",
                    "HttpTokenEndpoint - doPost - xml body content expected");
    try {
      // read XML request body in string stream
      boost::property_tree::read_xml(l_stream, requestBodyTree);
    } catch (const std::exception &ex) {
      m_logger->error(
          "HTTP_DATA_READ",
          "HttpTokenEndpoint - doPost - XML body has invalid structure" +
              std::string(ex.what()));
      throw ParsingException("body has invalid structure");
    }
  } else {
    m_logger->error("HTTP_DATA_READ",
                    "HttpTokenEndpoint - doPost - content type not supported");
    throw ParsingException("content type is not valid");
  }
  try {
    const std::string l_login = requestBodyTree.get<std::string>("login");
    m_logger->debug("HTTP_DATA_READ", "HttpTokenEndpoint - doPost - login :" +
                                          std::string(l_login));
    const std::string l_pwd = requestBodyTree.get<std::string>("password");
    m_logger->debug("HTTP_DATA_READ",
                    "HttpTokenEndpoint - doPost - password :" +
                        std::string(l_pwd));
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
    // ERROR TYPE : ENOKEY, mandatory key not found
    // FIELD : [NAME, ERROR]
    m_logger->error("HTTP_DATA_READ",
                    "HttpTokenEndpoint - doPost - body has at least one "
                    "mandatory field : " +
                        std::string(ex.what()));
    throw ParsingException("body has at least one mandatory field");
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_data> &ex) {
    // ERROR TYPE : EVALUETYPE, key found with bad value type
    // FIELD : [NAME, ERROR]
    m_logger->error("HTTP_DATA_READ",
                    "HttpTokenEndpoint - doPost - body has bad value type : " +
                        std::string(ex.what()));
    throw ParsingException("body has bad value type");
  }

  boost::beast::http::response<boost::beast::http::string_body> res{
      boost::beast::http::status::ok, request.version()};
  res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(boost::beast::http::field::content_type, "text/plain");
  res.keep_alive(request.keep_alive());
  res.body() = "This is a fake token";
  res.prepare_payload();
  setResponse(res);
  m_logger->debug("HTTP_DATA_READ", "HttpTokenEndpoint - doPost - end");
}

/**
 * @brief Destroy the Http Token Endpoint:: Http Token Endpoint object
 *
 */
HttpTokenEndpoint::~HttpTokenEndpoint() = default;
