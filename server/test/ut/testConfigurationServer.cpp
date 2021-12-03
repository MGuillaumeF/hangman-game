#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testHttpConfigurationServer
#include "../../src/common/HTTP/testHttpConfigurationServer.hpp"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(testConfigurationServer)

BOOST_AUTO_TEST_CASE(testHttpConfigurationServer) {
  auto config = http::ConfigurationServer;
  BOOST_CHECK(config.getDescription().equals("this is description"));
}

BOOST_AUTO_TEST_SUITE_END()
