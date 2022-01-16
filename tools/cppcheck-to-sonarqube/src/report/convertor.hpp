#ifndef __CONVERTOR_HPP__
#define __CONVERTOR_HPP__

// boost import
#include <boost/property_tree/ptree.hpp>

// STL import
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
  static auto readCppCheckReport(const std::string &filename) -> boost::property_tree::ptree;

  /**
   * @brief method to convert the report
   *
   * @param cppCheckTree The readed cppcheck report ptree
   * @return boost::property_tree::ptree The converted ptree
   */
  static auto cppCheckToSonarReport(const boost::property_tree::ptree &cppCheckTree) -> boost::property_tree::ptree;

  /**
   * @brief method to convert the clang-tidy report
   *
   * @param filename The input clang-tidy report path
   * @return boost::property_tree::ptree The converted ptree
   */
  static auto clangTidyToSonarReport(const std::string &filename) -> boost::property_tree::ptree;
};

#endif
