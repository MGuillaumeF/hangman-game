#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../src/common/HTTP/Configuration/ConfigurationServer.hpp"
#include "../../src/common/HTTP/Exception/ConfigurationException.hpp"

BOOST_AUTO_TEST_SUITE(testConfigurationServer)

BOOST_AUTO_TEST_CASE(testHttpConfigurationServer) {
  auto config = ConfigurationServer();
  BOOST_CHECK(0 == config.getDescription().compare(
                       "example of server 1 configuration"));
  BOOST_CHECK(0 == config.getHostname().compare("0.0.0.0"));
  BOOST_CHECK(0 == config.getRoot().compare("."));
  BOOST_CHECK_EQUAL(8080, config.getPort());
  BOOST_CHECK_EQUAL(1, config.getThreads());
}

bool configurationErrorMessage(const ConfigurationException &ex) {
  BOOST_CHECK_EQUAL(ex.what(),
                    std::string("HTTP Configuration file is invalid"));
  return true;
}

BOOST_AUTO_TEST_CASE(bad_configuration_content) {
  BOOST_CHECK_EXCEPTION(ConfigurationServer("./configuration/mime-types.xml"),
                        ConfigurationException, configurationErrorMessage);
}

BOOST_AUTO_TEST_SUITE_END()
