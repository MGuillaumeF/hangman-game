#ifndef __HTTP_TOKEN_ENDPOINT_HPP__
#define __HTTP_TOKEN_ENDPOINT_HPP__
#include "HttpRestrictiveEndpoint.hpp"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../common/HTTP/Exception/ParsingException.hpp"
/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpTokenEndpoint : public HttpRestrictiveEndpoint {

public:
  HttpTokenEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req)
      : HttpRestrictiveEndpoint(req,
                                {{boost::beast::http::verb::post, true},
                                 {boost::beast::http::verb::get, false},
                                 {boost::beast::http::verb::put, false},
                                 {boost::beast::http::verb::patch, false},
                                 {boost::beast::http::verb::delete_, true}}) {

    std::cout << "HttpTokenEndpoint - constructor" << std::endl;
  }
  void doPost() override {
    std::cout << "HttpTokenEndpoint - doPost - start" << std::endl;

    std::stringstream l_stream(m_request.body());
    boost::property_tree::ptree requestRodyTree;

    const boost::string_view contentType =
        m_request.at(boost::beast::http::field::content_type);

    if (contentType.compare("application/json") == 0) {
      std::cout << "HttpTokenEndpoint - doPost - json body content expected"
                << std::endl;
      try {
        boost::property_tree::read_json(l_stream, requestRodyTree);
      } catch (const std::exception &ex) {
        std::cerr
            << "HttpTokenEndpoint - doPost - JSON body has invalid structure"
            << ex.what() << std::endl;
        throw ParsingException("body has invalid structure");
      }
    } else if (contentType.compare("application/xml") == 0) {
      std::cout << "HttpTokenEndpoint - doPost - xml body content expected"
                << std::endl;
      try {
        boost::property_tree::read_xml(l_stream, requestRodyTree);
      } catch (const std::exception &ex) {
        std::cerr
            << "HttpTokenEndpoint - doPost - XML body has invalid structure"
            << ex.what() << std::endl;
        throw ParsingException("body has invalid structure");
      }
    } else {
      std::cout << "HttpTokenEndpoint - doPost - content type not supported"
                << std::endl;
      throw ParsingException("content type is not valid");
    }
    try {
      const std::string l_login = requestRodyTree.get<std::string>("login");
      std::cout << "HttpTokenEndpoint - doPost - login :" << l_login
                << std::endl;
      const std::string l_pwd = requestRodyTree.get<std::string>("password");
      std::cout << "HttpTokenEndpoint - doPost - password :" << l_pwd
                << std::endl;
    } catch (
        const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
      // ERROR TYPE : ENOKEY, mandatory key not found
      // FIELD : [NAME, ERROR]
      std::cerr << "HttpTokenEndpoint - doPost - body has at least one "
                   "mandatory field : "
                << ex.what() << std::endl;
      throw ParsingException("body has at least one mandatory field");
    } catch (
        const boost::wrapexcept<boost::property_tree::ptree_bad_data> &ex) {
      // ERROR TYPE : EVALUETYPE, key found with bad value type
      // FIELD : [NAME, ERROR]
      std::cerr << "HttpTokenEndpoint - doPost - body has bad value type : "
                << ex.what() << std::endl;
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
    std::cout << "HttpTokenEndpoint - doPost - end" << std::endl;
  }
};

#endif // __HTTP_TOKEN_ENDPOINT_HPP__