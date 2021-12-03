#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HangmanGameTest
#define BOOST_TEST_NO_NAME

#include <boost/test/init_test.hpp>

book init_function() {
  // before all
  boost::unit_test::framework::master_test_suite().p_name.value = "HangmanGameTestSuite";
  return true;
}

int main() {
  int all_test_result = boost::unit_test::unit_test_main(&init_function, &argc, &atgv);
  return all_test_result;
}
