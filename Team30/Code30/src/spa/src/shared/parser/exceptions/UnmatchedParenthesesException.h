#pragma once

#include <stdexcept>
#include <string>

class UnmatchedParenthesesException : public std::invalid_argument {
  /**
   * Thrown when there is an unmatched parentheses.
   */
 public:
  explicit UnmatchedParenthesesException(
      std::string const& what_arg = "Unmatched parentheses!");
};