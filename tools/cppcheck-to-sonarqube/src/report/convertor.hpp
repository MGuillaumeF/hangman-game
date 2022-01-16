#ifndef __CONVERTOR_HPP__
#define __CONVERTOR_HPP__

#include <boost/property_tree/ptree.hpp>
#include <string>

/**
 * @brief SonarCloud severity levels of issues
 *
 */
enum class ESonarCloudSeverity { INFO, MINOR, MAJOR, CRITICAL, BLOCKER };

/**
 * @brief class to convert cppcheck xml report to sonarqube json report
 *
 */
class Convertor {
public:
  /**
   * @brief Construct a new Convertor object
   *
   */
  Convertor() = delete;

  /**
   * @brief method to read cppcheck
   *
   * @param filename The input cppcheck report path
   * @return boost::property_tree::ptree The readed cppcheck report ptree
   */
  static boost::property_tree::ptree
  readCppCheckReport(const std::string &filename);

  /**
   * @brief method to convert the report
   *
   * @param cppCheckTree The readed cppcheck report ptree
   * @return boost::property_tree::ptree The converted ptree
   */
  static boost::property_tree::ptree cppCheckToSonarReport(
      const boost::property_tree::ptree &cppCheckTree);

  /**
   * @brief method to convert the clang-tidy report
   *
   * @param filename The input clang-tidy report path
   * @return boost::property_tree::ptree The converted ptree
   */
  static boost::property_tree::ptree
  clangTidyToSonarReport(const std::string &filename);
};

#endif
