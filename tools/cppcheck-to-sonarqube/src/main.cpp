#include "report/convertor.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
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
    boost::property_tree::ptree sonarqubeReport;
    if (argc == 4) {
      if (0 == std::string("cppcheck").compare(argv[1])) {
        boost::property_tree::ptree cppCheckReport =
            Convertor::readCppCheckReport(argv[2]);
        sonarqubeReport =
            Convertor::cppCheckReportToSonarqubeReportTree(cppCheckReport);
       
      } else if (0 == std::string("clang-tidy").compare(argv[1])) {
        sonarqubeReport =
            Convertor::clangTidyReportToSonarqubeReportTree(argv[2]);
      }
    } else {
      std::cerr << "Bad usage of tool : " << std::endl
                << "  example of expected usage : cppcheck-to-sonarqube "
                   "cppcheck-report.xml cppcheck-sonarqube-report.json";
    }
    if (sonarqubeReport.get_child("issues").size() > 0) {
      boost::property_tree::write_json(argv[3], sonarqubeReport);
    } else {
      std::ofstream emptyJsonFile;
      emptyJsonFile.open(argv[3]);
      emptyJsonFile << "{\"issues\": []}";
      emptyJsonFile.close();
    }
  } catch (const std::exception &ex) {
    std::cerr << "Error : " << ex.what() << std::endl;
  }
}
