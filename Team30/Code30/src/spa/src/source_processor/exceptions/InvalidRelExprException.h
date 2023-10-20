#pragma once

#include <stdexcept>
#include <string>

class InvalidRelExprException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid rel expression.
   */
 public:
  explicit InvalidRelExprException(
      std::string const& what_arg = "Invalid rel expression!");
};
