#pragma once

#include <stdexcept>
#include <string>

class InvalidSemanticsException : public std::invalid_argument {
  /**
   * Thrown when a query is semantically invalid.
   */
 public:
  explicit InvalidSemanticsException(std::string const& what_arg);
};
