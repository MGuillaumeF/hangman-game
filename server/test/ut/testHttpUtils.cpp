#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../src/common/HTTP/Utils.hpp"

BOOST_AUTO_TEST_SUITE(testsHttpUtils)

BOOST_AUTO_TEST_CASE(testMimeType) {
  std::string mimeType = http::Utils::getMimeType("test.html");
  BOOST_CHECK_EQUAL("text/html", mimeType);

  mimeType = http::Utils::getMimeType("./");
  BOOST_CHECK_EQUAL("application/text", mimeType);

  mimeType = http::Utils::getMimeType("");
  BOOST_CHECK_EQUAL("application/text", mimeType);

  mimeType = http::Utils::getMimeType("file/to/image.jpe");
  BOOST_CHECK_EQUAL("image/jpeg", mimeType);

  mimeType = http::Utils::getMimeType("file/to/textFile.txt");
  BOOST_CHECK_EQUAL("text/plain", mimeType);

  mimeType = http::Utils::getMimeType("file/to/image.gif");
  BOOST_CHECK_EQUAL("image/gif", mimeType);

  mimeType = http::Utils::getMimeType("file/to/file/without/extension");
  BOOST_CHECK_EQUAL("application/text", mimeType);
}

BOOST_AUTO_TEST_SUITE_END()
