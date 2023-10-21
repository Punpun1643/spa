#pragma once

#include <stdexcept>
#include <string>

class InvalidReadException : public std::invalid_argument {
  /**
   * Thrown when there is an invalid read stmt.
   */
 public:
  explicit InvalidReadException(
      std::string const& what_arg = "Invalid read statement!");
};
