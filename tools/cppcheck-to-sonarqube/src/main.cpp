#include "report/convertor.hpp"

// boost imports
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

// STL stream import
#include <fstream>
#include <iostream>

// STL data containers import
#include <list>

/**
 * @brief entry of application
 *
 * @param argc The number of argument of process launch command
 * @param argv The process launch command arguments array
 * @return int The process exit code
 */
int32_t main(int argc, char *argv[]) {
  // by default if execution don't raise error
  // the exit code is success
  int32_t exitStatus = EXIT_SUCCESS;

  // process args expected
  const uint8_t ARGUMENTS_SIZE = 4;

  try {
    boost::property_tree::ptree sonarqubeReport;
    if (argc == ARGUMENTS_SIZE) {
      std::list<std::string> args(argv + 1, argv + argc);

      const std::string reportType = args.front();
      args.pop_front();
      const std::string inputFile = args.front();
      args.pop_front();
      const std::string outputFile = args.front();
      args.pop_front();

      if (reportType == "cppcheck") {
        const boost::property_tree::ptree cppCheckReport =
            Convertor::readCppCheckReport(inputFile);
        sonarqubeReport =
            Convertor::cppCheckToSonarReport(cppCheckReport);
       
      } else if (reportType == "clang-tidy") {
        sonarqubeReport =
            Convertor::clangTidyToSonarReport(inputFile);
      }
      // if at least one issue found write property tree json report
      if (!sonarqubeReport.get_child("issues").empty()) {
        boost::property_tree::write_json(outputFile, sonarqubeReport);
      } else {
        // if issues property tree array is empty write empty report 
        // to have a valid report
        std::ofstream emptyJsonFile;
        emptyJsonFile.open(outputFile);
        emptyJsonFile << "{\"issues\": []}";
        emptyJsonFile.close();
      }
    } else {
      std::cerr << "Bad usage of tool : " << std::endl
                << "  example of expected usage : cppcheck-to-sonarqube "
                   "cppcheck-report.xml cppcheck-sonarqube-report.json";
    }
  // catch all exception to have a managed return with error log
  } catch (const std::exception &ex) {
    std::cerr << "Unexpected Fatal Error : " << ex.what() << std::endl;
    // change process return value
    exitStatus = EXIT_FAILURE;
  }
  return exitStatus;
}
