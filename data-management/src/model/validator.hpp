#ifndef __VALIDATOR_HPP__
#define __VALIDATOR_HPP__

#include "./model_error.hpp"

struct string_constraint {
  bool mandatory;
  uint64_t max_length;
  uint64_t min_length;
  std::string pattern;
};

struct number_constraint {
  bool mandatory;
  uint64_t max;
  int64_t min;
};

class validator {
public:
  /**
   * method to valid a string property by constraint
   * @param constraint The constraint to have a valid data
   * @param attribute_name The attribute name of object
   * @param value The value to check
   * @returns The list of error of data check
   */
  static std::vector<model_error>
  check_string_property(number_constraint constraint,
                        std::string attribute_name, uint64_t value) {
    std::vector<model_error> errors;
    return errors;
  }
  
  /**
   * method to valid a number property by constraint
   * @param constraint The constraint to have a valid data
   * @param attribute_name The attribute name of object
   * @param value The value to check
   * @returns The list of error of data check
   */
  static std::vector<model_error>
  check_number_property(number_constraint constraint,
                        std::string attribute_name, uint64_t value) {
    std::vector<model_error> errors;
    return errors;
  }
};

#endif
