#pragma once

#include <string>

class InvalidContainerStmtException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid if/while stmt.
   */
 public:
  explicit InvalidContainerStmtException(
      std::string const& what_arg = "Invalid if/while statement!");
};