#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include "../../src/common/HTTP/Utils.hpp"

BOOST_AUTO_TEST_SUITE(testsHttpUtils)

BOOST_AUTO_TEST_CASE(testMimeType) {
  boost::beast::string_view mimeType = http::Utils::getMimeType("test.html");
  BOOST_CHECK(mimeType.compare("text/html"));

  mimeType = http::Utils::getMimeType("./");
  BOOST_CHECK(mimeType.compare("application/text"));

  mimeType = http::Utils::getMimeType("");
  BOOST_CHECK(mimeType.compare("application/text"));

  mimeType = http::Utils::getMimeType("file/to/image.jpe");
  BOOST_CHECK(mimeType.compare("image/jpeg"));

  mimeType = http::Utils::getMimeType("file/to/textFile.txt");
  BOOST_CHECK(mimeType.compare("text/plain"));

  mimeType = http::Utils::getMimeType("file/to/image.gif");
  BOOST_CHECK(mimeType.compare("image/gif"));

  mimeType = http::Utils::getMimeType("file/to/file/without/extension");
  BOOST_CHECK(mimeType.compare("application/text"));
}

BOOST_AUTO_TEST_SUITE_END()
