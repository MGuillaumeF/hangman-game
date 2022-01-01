#include "report/convertor.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

/**
 * @brief
 *
 * @param argc The number of argument of process launch command
 * @param argv The process launch command arguments array
 * @return int The process exit code
 */
int main(int argc, char *argv[]) {
  try {
    if (argc == 3) {
      boost::property_tree::ptree cppCheckReport =
          Convertor::readCppCheckReport(argv[1]);
      boost::property_tree::ptree sonarqubeReport =
          Convertor::cppCheckReportToSonarqubeReportTree(cppCheckReport);
      boost::property_tree::write_json(argv[2], sonarqubeReport);
    } else {
      std::cerr << "Bad usage of tool : " << std::endl
                << "  example of expected usage : cppcheck-to-sonarqube "
                   "cppcheck-report.xml cppcheck-sonarqube-report.json";
    }
  } catch (const std::exception &ex) {
    std::cerr << "Error : " << ex.what() << std::endl;
  }
}