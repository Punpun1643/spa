#pragma once

#include <string>

class InvalidExprException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid expression.
   */
 public:
  explicit InvalidExprException(
      std::string const& what_arg =
          "Invalid expression: insufficient operands for operator");
};