#pragma once

#include <string>

class InvalidSourceSemanticsException : public std::invalid_argument {
  /**
   * Thrown when a query is semantically invalid.
   */
 public:
  explicit InvalidSourceSemanticsException(std::string const& what_arg);
};
