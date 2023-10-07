#pragma once

#include <string>

class InvalidCallException : public std::invalid_argument {
  /**
   * Thrown when a there is an invalid call stmt.
   */
 public:
  explicit InvalidCallException(std::string const& what_arg = "Invalid call statement!");
};