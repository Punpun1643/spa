#pragma once

#include <stdexcept>
#include <string>

class InvalidSyntaxException : public std::invalid_argument {
  /**
   * Thrown when a query is syntactically invalid.
   */
 public:
  explicit InvalidSyntaxException(std::string const& what_arg);
};
