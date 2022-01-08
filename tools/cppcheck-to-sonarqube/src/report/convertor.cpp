#include "convertor.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>

/**
 * @brief SonarCloud severity levels of issues
 *
 */
enum SonarCloudSeverity { INFO, MINOR, MAJOR, CRITICAL, BLOCKER };

/**
 * @brief map to convert cppcheck severity to sonacloud severity
 *
 */
const std::map<std::string, SonarCloudSeverity> cppcheckToSonarCloudSeverity = {
    {"none", INFO},
    {"debug", INFO},
    {"information", INFO},
    {"style", MINOR},
    {"warning", MAJOR},
    {"portability", MAJOR},
    {"performance", CRITICAL},
    {"error", BLOCKER}};

/**
 * @brief map to convert cppcheck severity to sonacloud severity
 *
 */
const std::map<SonarCloudSeverity, std::string> SonarCloudSeverityValue = {
    {INFO, "INFO"},
    {MINOR, "MINOR"},
    {MAJOR, "MAJOR"},
    {CRITICAL, "CRITICAL"},
    {BLOCKER, "BLOCKER"}};

/**
 * @brief Method to read cppcheck file
 *
 * @param filename The input cppcheck report path
 * @return boost::property_tree::ptree The readed cppcheck report ptree
 */
boost::property_tree::ptree
Convertor::readCppCheckReport(const std::string &filename) {
  boost::property_tree::ptree cppCheckReport;
  boost::property_tree::read_xml(filename, cppCheckReport);
  return cppCheckReport;
}

/**
 * @brief method to convert the report
 *
 * @param cppCheckTree The readed cppcheck report ptree
 * @return boost::property_tree::ptree The converted ptree
 */
boost::property_tree::ptree Convertor::cppCheckReportToSonarqubeReportTree(
    const boost::property_tree::ptree &cppCheckTree) {
  boost::property_tree::ptree sonarQubeReport;

  // get cppcheck version to generate engineId of issues
  const std::string cppckecVerison =
      cppCheckTree.get<std::string>("results.cppcheck.<xmlattr>.version");

  const boost::property_tree::ptree errors =
      cppCheckTree.get_child("results.errors");

  boost::property_tree::ptree issues;

  // for each error
  for (const auto &error : errors) {
    boost::property_tree::ptree issue;
    // generate engineId with cppcheck version
    issue.put<std::string>("engineId", "cppcheck-" + cppckecVerison);
    const std::string ruleId = error.second.get<std::string>("<xmlattr>.id");
    const std::string severity =
        error.second.get<std::string>("<xmlattr>.severity");

    // the ruleId is the error Id
    issue.put<std::string>("ruleId", ruleId);

    // The severity is calculated and translated
    issue.put<std::string>(
        "severity",
        SonarCloudSeverityValue.at(cppcheckToSonarCloudSeverity.at(severity)));

    // all issues have type CODE_SMELL
    issue.put<std::string>("type", "CODE_SMELL");
    const std::string message = error.second.get<std::string>("<xmlattr>.msg");

    bool first = true;
    boost::property_tree::ptree secondaryLocations;
    for (const auto &errorChild : error.second) {
      if (errorChild.first == "location") {
        const std::string filePath =
            errorChild.second.get<std::string>("<xmlattr>.file");

        boost::property_tree::ptree location;
        location.put<std::string>("message", message);
        location.put<std::string>("filePath", filePath);

        // add text range to get location of error
        boost::property_tree::ptree textRange;
        const std::string line = errorChild.second.get<std::string>("<xmlattr>.line");
        textRange.put<std::string>("startLine", line == "0" ? "1" : line);
        textRange.put<std::string>(
            "startColumn",
            errorChild.second.get<std::string>("<xmlattr>.column"));
        location.add_child("textRange", textRange);

        // if it's first occurency of error add in primary place
        if (first) {
          issue.add_child("primaryLocation", location);
          first = false;
        } else {
          // else add in secondary place
          secondaryLocations.push_back(
              std::pair<const std::string, boost::property_tree::ptree>(
                  "", location));
        }
      }
    }

    // add secondary locations if ptree is not empty
    if (secondaryLocations.size() > 0) {
      issue.add_child("secondaryLocations", secondaryLocations);
    }

    // if the issue haven't primaryLocation we don't push the issue because it's
    // a mandatory field
    if (!first) {
      issues.push_back(
          std::pair<const std::string, boost::property_tree::ptree>("", issue));
    }
  }

  sonarQubeReport.add_child("issues", issues);

  return sonarQubeReport;
}

/**
 * @brief method to convert the clang-tidy report
 *
 * @param filename The input clang-tidy report path
 * @return boost::property_tree::ptree The converted ptree
 */
boost::property_tree::ptree
Convertor::clangTidyReportToSonarqubeReportTree(const std::string &filename) {

  boost::property_tree::ptree sonarQubeReport;
  std::ifstream inFile;
  // open the input file
  inFile.open(filename);

  std::stringstream strStream;
  // read the file
  strStream << inFile.rdbuf();
  // str holds the content of the file
  const std::string reportContent = strStream.str();

  const std::regex regex("(.+\\.[ch]pp):(\\d+):(\\d+):(.+):(.+)\\[(.+)\\]");

  // std::smatch match;
  // if (regex_search(reportContent, match, regex) == true) {

  //       std::cout << "Match size = " << match.size() << std::endl;

  //       std::cout << "Whole match : " << match.str(0) << std::endl;
  //       std::cout << "Filename is :" << match.str(1)  << std::endl
  //            << "Line :" <<  match.str(2)  << std::endl
  //            << "Column :" <<  match.str(3)  << std::endl
  //            << "Severity :" <<  match.str(4)  << std::endl
  //            << "Message :" <<  match.str(5)  << std::endl
  //            << "ruleId :" <<  match.str(6)
  //            << std::endl;
  //   }
  //   else {
  //      std::cout << "No match is found" << std::endl;
  //   }

  for (std::sregex_iterator i = std::sregex_iterator(
           reportContent.begin(), reportContent.end(), regex);
       i != std::sregex_iterator(); ++i) {
    std::smatch match = *i;
    std::cout << "Match size = " << match.size() << std::endl;

    std::cout << "Whole match : " << match.str(0) << std::endl;
    std::cout << "Filename is :" << match.str(1) << std::endl
              << "Line :" << match.str(2) << std::endl
              << "Column :" << match.str(3) << std::endl
              << "Severity :" << match.str(4) << std::endl
              << "Message :" << match.str(5) << std::endl
              << "ruleId :" << match.str(6) << std::endl;
  }
  return sonarQubeReport;
}
