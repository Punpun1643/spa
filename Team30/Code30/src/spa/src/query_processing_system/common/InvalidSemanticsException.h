#pragma once
#include <stdexcept>

class InvalidSemanticsException : public std::invalid_argument {
  /**
   * Thrown when a Clause is given arguments that make it semantically invalid.
   */
 public:
  explicit InvalidSemanticsException(const std::string& what_arg);
};
