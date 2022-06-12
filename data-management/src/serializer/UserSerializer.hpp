
#if defined(DATABASE_MYSQL)
#include "./model/mysql/user.hxx"
#elif defined(DATABASE_SQLITE)
#include "./model/sqlite/user.hxx"
#elif defined(DATABASE_PGSQL)
#include "./model/pgsql/user.hxx"
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include <boost/property_tree/xml_parser.hpp>
#include <string>

inline std::string toXml(const user &usr) { return ""; }

inline user parseXml(const std::string &jsonStr) {
  std::ostream ss(jsonStr);
  boost::property_tree::ptree pt;
  boost::property_tree::read_xml(ss, pt);
  user newUser();
  newUser.setLogin(pt.get<std::string>("login"));
  newUser.setPassword(pt.get<std::string>("password"));
  newUser.setSaltUser(pt.get<std::string>("salt_user"));
  return newUser;
}