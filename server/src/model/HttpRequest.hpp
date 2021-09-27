#ifndef __HTTP_REQUEST_HPP__
#define __HTTP_REQUEST_HPP__

#include <boost/beast/http/fields.hpp>
#include <boost/beast/http/verb.hpp>
#include <map>
#include <string>
struct HttpRequest {
  std::string target = "/";
  std::string body = "";
  std::map<std::string, std::string> headers = {};
  std::string version = "1.1";
  boost::beast::http::verb method = boost::beast::http::verb::get;
  bool keepAlive = true;
};
#endif