#pragma once

#include <string>

class InvalidPrintException : public std::invalid_argument {
  /**
   * Thrown when a there is an invalid print stmt.
   */
 public:
  explicit InvalidPrintException(
      std::string const& what_arg = "Invalid print statement!");
};