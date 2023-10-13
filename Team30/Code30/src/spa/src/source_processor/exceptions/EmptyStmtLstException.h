#pragma once

#include <stdexcept>
#include <string>

class EmptyStmtLstException : public std::invalid_argument {
  /**
   * Thrown when the program has empty StmtLst.
   */
 public:
  explicit EmptyStmtLstException(
      std::string const& what_arg = "Empty StmtLst!");
};