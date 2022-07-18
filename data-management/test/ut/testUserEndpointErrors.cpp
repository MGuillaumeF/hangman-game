#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <exception>
#include <fstream>
#include <iostream> // std::cout
#include <memory>   // unique_ptr
#include <thread>

#include <chrono>
#include <iomanip>
#include <sstream> // std::stringstream
#include <string>  // std::string

#include <boost/property_tree/ptree.hpp>

#include "../../src/TCP/Server.hpp"

BOOST_AUTO_TEST_SUITE(testUserEndpointErrors)

BOOST_AUTO_TEST_CASE(test_create_duplicated_user_by_name) {
  // open transaction
  // count user as A
  // create user as TOTO
  // count user as B
  // except B == A + 1
  // close transaction

  // open transaction
  // count user as A
  // create user as TOTO
  // Error raised : TOTO already exist
  // close transaction
}

BOOST_AUTO_TEST_CASE(test_read_user_not_found) {
  // open transaction
  // search user TUTU
  // Error raised : TUTU not found
  // close transaction
}

BOOST_AUTO_TEST_CASE(test_update_invalid_update) {
  // open transaction
  // search user TOTO
  // found TOTO and all attributs
  // update email with bad pattern
  // Error raised : invalid pattern of new value of email
  // close transaction
}

BOOST_AUTO_TEST_CASE(test_delete_user_not_found) {
  // open transaction
  // delete TUTU
  // Error raised : TUTU not found
  // close transaction
}

BOOST_AUTO_TEST_SUITE_END()