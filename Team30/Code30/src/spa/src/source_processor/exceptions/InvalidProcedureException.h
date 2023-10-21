#pragma once

#include <stdexcept>
#include <string>

class InvalidProcedureException : public std::invalid_argument {
  /**
   * Thrown when a there is an invalid procedure.
   */
 public:
  explicit InvalidProcedureException(
      std::string const& what_arg = "Invalid procedure!");
};
