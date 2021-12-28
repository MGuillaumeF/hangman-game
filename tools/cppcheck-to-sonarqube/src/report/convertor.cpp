#include "convertor.hpp"
#include <boost/property_tree/xml_parser.hpp>

boost::property_tree::ptree
Convertor::readCppCheckReport(const std::string &filename) {
  boost::property_tree::ptree cppCheckReport;
  boost::property_tree::read_xml(filename, cppCheckReport);
  return cppCheckReport;
}
boost::property_tree::ptree Convertor::cppCheckReportToSonarqubeReportTree(
    const boost::property_tree::ptree &cppCheckTree) {
  boost::property_tree::ptree sonarQubeReport;

  return sonarQubeReport;
}