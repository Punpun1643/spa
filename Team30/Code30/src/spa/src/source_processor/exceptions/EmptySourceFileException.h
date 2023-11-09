#pragma once

#include <stdexcept>
#include <string>

class EmptySourceFileException : public std::invalid_argument {
  /**
   * Thrown when the program has empty source file.
   */
 public:
  explicit EmptySourceFileException(
      std::string const& what_arg = "Empty source file!");
};
