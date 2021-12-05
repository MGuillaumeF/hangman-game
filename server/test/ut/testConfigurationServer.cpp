#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../src/common/HTTP/Configuration/ConfigurationServer.hpp"

BOOST_AUTO_TEST_SUITE(testConfigurationServer)

BOOST_AUTO_TEST_CASE(testHttpConfigurationServer) {
  auto config = ConfigurationServer({});
  BOOST_CHECK(0 == config.getDescription().compare("example of server 1 configuration"));
  BOOST_CHECK(0 == config.getHostname().compare("localhost"));
  BOOST_CHECK(0 == config.getRoot().compare("."));
  BOOST_CHECK_EQUAL(80, config.getPort());
  BOOST_CHECK_EQUAL(1, config.getThreads());
}

BOOST_AUTO_TEST_SUITE_END()
