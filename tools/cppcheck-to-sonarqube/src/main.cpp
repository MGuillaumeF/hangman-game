#include "report/convertor.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

/**
 * @brief
 *
 * @param argc The number of argument of process launch command
 * @param argv The process launch command arguments array
 * @return int The process exit code
 */
int main(int argc, char *argv[]) {
  boost::property_tree::ptree cppCheckReport =
      Convertor::readCppCheckReport(argv[1]);
  boost::property_tree::ptree sonarqubeReport =
      Convertor::cppCheckReportToSonarqubeReportTree(cppCheckReport);
  boost::property_tree::write_json(argv[1], sonarqubeReport);
}