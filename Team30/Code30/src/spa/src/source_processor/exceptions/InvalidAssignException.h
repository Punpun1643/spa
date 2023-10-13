#pragma once

#include <string>

class InvalidAssignException : public std::invalid_argument {
  /**
   * Thrown when a there is an invalid assign stmt.
   */
 public:
  explicit InvalidAssignException(
      std::string const& what_arg = "Invalid assign statement!");
};