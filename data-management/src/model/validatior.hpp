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
    static std::vector<model_error> check_number_property(number_constraint constraint, std::string attribute_name,
    uint64_t value) {
      std::vector<model_error> errors;
      return errors;
    }

  
};

#endif
