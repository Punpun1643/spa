#pragma once

#include <string>

class InvalidCondExprException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid condition expression.
   */
 public:
  explicit InvalidCondExprException(
      std::string const& what_arg = "Invalid condition expression!");
};