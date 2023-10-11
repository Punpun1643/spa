#pragma once

#include <string>

class InvalidExprSpecException : public std::invalid_argument {
 public:
  explicit InvalidExprSpecException(
      std::string const& what_arg = "Invalid expression specification!");
};