#ifndef __HTTP_TOKEN_ENDPOINT_HPP__
#define __HTTP_TOKEN_ENDPOINT_HPP__
#include "HttpRestrictiveEndpoint.hpp"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../common/HTTP/Exception/ParsingException.hpp"
#include "../common/Logger/LoggerFile.hpp"

/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpTokenEndpoint : public HttpRestrictiveEndpoint {
private:
  LoggerFile *m_logger;

public:
  HttpTokenEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req)
      : HttpRestrictiveEndpoint(req,
                                {{boost::beast::http::verb::post, true},
                                 {boost::beast::http::verb::get, false},
                                 {boost::beast::http::verb::put, false},
                                 {boost::beast::http::verb::patch, false},
                                 {boost::beast::http::verb::delete_, true}}) {
    m_logger = LoggerFile::getInstance();
    m_logger->setLevel(LoggerFile::LEVEL::LINFO);

    m_logger->debug("HttpTokenEndpoint - constructor");
  }

  void doPost() override {
    m_logger->debug("HttpTokenEndpoint - doPost - start");

    std::stringstream l_stream(m_request.body());
    boost::property_tree::ptree requestRodyTree;

    const boost::string_view contentType =
        m_request.at(boost::beast::http::field::content_type);

    if (contentType.compare("application/json") == 0) {
      m_logger->debug(
          "HttpTokenEndpoint - doPost - json body content expected");
      try {
        boost::property_tree::read_json(l_stream, requestRodyTree);
      } catch (const std::exception &ex) {
        m_logger->error(
            "HttpTokenEndpoint - doPost - JSON body has invalid structure" +
            std::string(ex.what()));
        throw ParsingException("body has invalid structure");
      }
    } else if (contentType.compare("application/xml") == 0) {
      m_logger->debug("HttpTokenEndpoint - doPost - xml body content expected");
      try {
        boost::property_tree::read_xml(l_stream, requestRodyTree);
      } catch (const std::exception &ex) {
        m_logger->error(
            "HttpTokenEndpoint - doPost - XML body has invalid structure" +
            std::string(ex.what()));
        throw ParsingException("body has invalid structure");
      }
    } else {
      m_logger->error(
          "HttpTokenEndpoint - doPost - content type not supported");
      throw ParsingException("content type is not valid");
    }
    try {
      const std::string l_login = requestRodyTree.get<std::string>("login");
      m_logger->debug("HttpTokenEndpoint - doPost - login :" +
                      std::string(l_login));
      const std::string l_pwd = requestRodyTree.get<std::string>("password");
      m_logger->debug("HttpTokenEndpoint - doPost - password :" +
                      std::string(l_pwd));
    } catch (
        const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
      // ERROR TYPE : ENOKEY, mandatory key not found
      // FIELD : [NAME, ERROR]
      m_logger->error("HttpTokenEndpoint - doPost - body has at least one "
                      "mandatory field : " +
                      std::string(ex.what()));
      throw ParsingException("body has at least one mandatory field");
    } catch (
        const boost::wrapexcept<boost::property_tree::ptree_bad_data> &ex) {
      // ERROR TYPE : EVALUETYPE, key found with bad value type
      // FIELD : [NAME, ERROR]
      m_logger->error(
          "HttpTokenEndpoint - doPost - body has bad value type : " +
          std::string(ex.what()));
      throw ParsingException("body has bad value type");
    }

    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::ok, m_request.version()};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, "text/plain");
    res.keep_alive(m_request.keep_alive());
    res.body() = "This is a fake token";
    res.prepare_payload();
    setResponse(res);
    m_logger->debug("HttpTokenEndpoint - doPost - end");
  }
};

#endif // __HTTP_TOKEN_ENDPOINT_HPP__