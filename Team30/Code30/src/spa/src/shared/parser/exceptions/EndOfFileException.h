#pragma once

#include <stdexcept>
#include <string>

class EndOfFileException : public std::runtime_error {
 public:
  explicit EndOfFileException(
      std::string const& errorMessage = "No more tokens available!");
};