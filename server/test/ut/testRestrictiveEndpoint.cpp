#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <list>
#include <string>

#include "../../src/common/HTTP/RestrictiveEndpoint.hpp"

const std::string expected_body_content_501 = "The method is not implemented.";
const std::string expected_body_content_405 = "The method is not allowed.";

struct request_test {
  std::string message;
  boost::beast::http::verb method;
  unsigned int expected_status;
  std::string expected_body_content;
};

std::list<request_test> requests_tests = {
    {
      message : "RestrictiveEndpoint test POST not implemented method",
      method : boost::beast::http::verb::post,
      expected_status : 501U,
      expected_body_content : expected_body_content_501
    },
    {
      message : "RestrictiveEndpoint test GET not implemented method",
      method : boost::beast::http::verb::get,
      expected_status : 501U,
      expected_body_content : expected_body_content_501
    },
    {
      message : "RestrictiveEndpoint test PUT not implemented method",
      method : boost::beast::http::verb::put,
      expected_status : 501U,
      expected_body_content : expected_body_content_501
    },
    {
      message : "RestrictiveEndpoint test PATCH not allowed method",
      method : boost::beast::http::verb::patch,
      expected_status : 405U,
      expected_body_content : expected_body_content_405
    },
    {
      message : "RestrictiveEndpoint test DELETE not implemented method",
      method : boost::beast::http::verb::delete_,
      expected_status : 501U,
      expected_body_content : expected_body_content_501
    }};

BOOST_AUTO_TEST_SUITE(testsRestrictiveEndpoint)

BOOST_AUTO_TEST_CASE(testsRestrictiveEndpointMethods) {
  boost::beast::http::request<boost::beast::http::string_body> request;
  request.target("/");
  for (const request_test &rt : requests_tests) {

    request.method(rt.method);
    http::RestrictiveEndpoint restrictiveEndpoint(
        request,
        {boost::beast::http::verb::post, boost::beast::http::verb::get,
         boost::beast::http::verb::put, boost::beast::http::verb::delete_});

    std::cout << "TEST CASE : " << rt.message << std::endl;
    restrictiveEndpoint.dispatchRequest();
    BOOST_CHECK_EQUAL(restrictiveEndpoint.getResponse().result_int(),
                      rt.expected_status);
    BOOST_CHECK_EQUAL(restrictiveEndpoint.getResponse().body(),
                      rt.expected_body_content);
  }
}

BOOST_AUTO_TEST_SUITE_END()
