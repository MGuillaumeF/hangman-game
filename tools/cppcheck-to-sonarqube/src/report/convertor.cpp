#include "convertor.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <map>

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
 * @param filename The path of cppcheck report file
 * @return boost::property_tree::ptree
 */
boost::property_tree::ptree
Convertor::readCppCheckReport(const std::string &filename) {
  boost::property_tree::ptree cppCheckReport;
  boost::property_tree::read_xml(filename, cppCheckReport);
  return cppCheckReport;
}

/**
 * @brief
 *
 * @param cppCheckTree
 * @return boost::property_tree::ptree
 */
boost::property_tree::ptree Convertor::cppCheckReportToSonarqubeReportTree(
    const boost::property_tree::ptree &cppCheckTree) {
  boost::property_tree::ptree sonarQubeReport;

  const std::string cppckecVerison =
      cppCheckTree.get<std::string>("results.cppcheck.<xmlattr>.version");

  const boost::property_tree::ptree errors =
      cppCheckTree.get_child("results.errors");

  boost::property_tree::ptree issues;

  for (const auto &error : errors) {
    boost::property_tree::ptree issue;
    issue.put<std::string>("engineId", "cppcheck-" + cppckecVerison);
    const std::string ruleId = error.second.get<std::string>("<xmlattr>.id");
    const std::string severity =
        error.second.get<std::string>("<xmlattr>.severity");
    issue.put<std::string>("ruleId", ruleId);
    issue.put<std::string>(
        "severity",
        SonarCloudSeverityValue.at(cppcheckToSonarCloudSeverity.at(severity)));
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

        boost::property_tree::ptree textRange;
        textRange.put<std::string>(
            "startLine", errorChild.second.get<std::string>("<xmlattr>.line"));
        textRange.put<std::string>(
            "startColumn",
            errorChild.second.get<std::string>("<xmlattr>.column"));
        location.add_child("textRange", textRange);

        if (first) {
          issue.add_child("primaryLocation", location);
          first = false;
        } else {
          secondaryLocations.push_back(
              std::pair<const std::string, boost::property_tree::ptree>(
                  "", location));
        }
      }
    }
    if (secondaryLocations.size() > 0) {
      issue.add_child("secondaryLocations", secondaryLocations);
    }
    issues.push_back(
        std::pair<const std::string, boost::property_tree::ptree>("", issue));
  }

  sonarQubeReport.add_child("issues", issues);

  return sonarQubeReport;
}