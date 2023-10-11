#pragma once

#include <stdexcept>
#include <string>

class StartOfFileException : public std::runtime_error {
 public:
  explicit StartOfFileException(
      std::string const& errorMessage = "No more tokens available!");
};