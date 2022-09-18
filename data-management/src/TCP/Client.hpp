#ifndef __TCP_CLIENT_HPP__
#define __TCP_CLIENT_HPP__

#include <string> // std::string

#include <boost/property_tree/ptree.hpp>

namespace hangman {
namespace tcp {
/**
 * @brief TCP Client class
 *
 */
class Client {
public:
  /**
   * @brief Construct a new TCP Client object
   *
   */
  Client() = delete;

  /**
   * method to send file with TCP Request to the data manager
   * @param hostname The hostname of data manager to join
   * @param port The port of data manager to join
   * @param filename The file to send over TCP request
   * @return The data manager response
   */
  static boost::property_tree::ptree sendRequest(const std::string &hostname,
                                                 const uint16_t &port,
                                                 const std::string &filename);
};

} // namespace tcp
} // namespace hangman
#endif // __TCP_CLIENT_HPP__
