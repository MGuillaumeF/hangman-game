#ifndef __CONVERTOR_HPP__
#define __CONVERTOR_HPP__

#include <boost/property_tree/ptree.hpp>
#include <string>

class Convertor {
public:
  Convertor() = delete;
  static boost::property_tree::ptree
  readCppCheckReport(const std::string &filename);
  static boost::property_tree::ptree cppCheckReportToSonarqubeReportTree(
      const boost::property_tree::ptree &cppCheckTree);
};

#endif