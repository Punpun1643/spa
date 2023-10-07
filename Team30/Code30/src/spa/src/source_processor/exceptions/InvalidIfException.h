#pragma once

#include <string>

class InvalidIfException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid if stmt.
   */
 public:
  explicit InvalidIfException(
      std::string const& what_arg = "Invalid if statement!");
};