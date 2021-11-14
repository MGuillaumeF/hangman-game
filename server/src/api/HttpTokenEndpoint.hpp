#ifndef __HTTP_TOKEN_ENDPOINT_HPP__
#define __HTTP_TOKEN_ENDPOINT_HPP__
#include "../common/HTTP/RestrictiveEndpoint.hpp"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../common/HTTP/Exception/ParsingException.hpp"
#include "../common/Logger/Logger.hpp"

/**
 * Http Endpoint Class ito create HTTP response
 */
class HttpTokenEndpoint : public http::RestrictiveEndpoint {

public:
  explicit HttpTokenEndpoint(
      const boost::beast::http::request<boost::beast::http::string_body> &req)
      : http::RestrictiveEndpoint(req,
                                {{boost::beast::http::verb::post, true},
                                 {boost::beast::http::verb::get, false},
                                 {boost::beast::http::verb::put, false},
                                 {boost::beast::http::verb::patch, false},
                                 {boost::beast::http::verb::delete_, true}});
  void doPost() override;
private:
  std::unique_ptr<Logger>& m_logger = Logger::getInstance();

};

#endif // __HTTP_TOKEN_ENDPOINT_HPP__
