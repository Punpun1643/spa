#pragma once

#include <stdexcept>
#include <string>

class EmptyParenthesesException : public std::invalid_argument {
  /**
   * Thrown when the program has empty parentheses.
   */
 public:
  explicit EmptyParenthesesException(
      std::string const& what_arg = "Empty parentheses!");
};