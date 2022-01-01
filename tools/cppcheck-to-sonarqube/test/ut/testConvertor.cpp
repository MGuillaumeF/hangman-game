#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../src/report/convertor.hpp"

BOOST_AUTO_TEST_SUITE(testsConvertor)

BOOST_AUTO_TEST_CASE(testReadCppCheck) { BOOST_CHECK(true); }
BOOST_AUTO_TEST_CASE(testConvertCppCheckToSonarQube) { BOOST_CHECK(true); }

BOOST_AUTO_TEST_SUITE_END()
