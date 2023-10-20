#pragma once

#include <stdexcept>
#include <string>

class InvalidStmtLstException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid stmtLst.
   */
 public:
  explicit InvalidStmtLstException(
      std::string const& what_arg = "Invalid stmtLst!");
};
