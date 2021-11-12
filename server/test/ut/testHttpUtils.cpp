#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testHttpUtils
#include "../../src/common/HTTP/Utils.hpp"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(testsHttpUtils)

BOOST_AUTO_TEST_CASE(testMimeType) {
  boost::beast::string_view mimeType = HTTP::Utils::getMimeType("test.html");
  BOOST_CHECK_EQUAL(mimeType, "text/html");

  mimeType = HTTP::Utils::getMimeType("./");
  BOOST_CHECK_EQUAL(mimeType, "application/text");

  mimeType = HTTP::Utils::getMimeType("");
  BOOST_CHECK_EQUAL(mimeType, "application/text");

  mimeType = HTTP::Utils::getMimeType("file/to/image.jpe");
  BOOST_CHECK_EQUAL(mimeType, "image/jpeg");

  mimeType = HTTP::Utils::getMimeType("file/to/textFile.txt");
  BOOST_CHECK_EQUAL(mimeType, "text/plain");

  mimeType = HTTP::Utils::getMimeType("file/to/image.gif");
  BOOST_CHECK_EQUAL(mimeType, "image/gif");

  mimeType = HTTP::Utils::getMimeType("file/to/file/without/extension");
  BOOST_CHECK_EQUAL(mimeType, "application/text");
}

BOOST_AUTO_TEST_SUITE_END()