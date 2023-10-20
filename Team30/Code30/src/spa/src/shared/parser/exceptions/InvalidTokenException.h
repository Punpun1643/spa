#pragma once

#include <stdexcept>
#include <string>

class InvalidTokenException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid token.
   */
 public:
  explicit InvalidTokenException(std::string const& what_arg);
};
