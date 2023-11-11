#pragma once

#include <string>

namespace QueryUtility {
bool IsIdentifier(std::string const& string);
bool IsValidQPInteger(std::string const& string);
bool IsQuotedIdentifier(std::string const& string);
bool IsSynonym(std::string const& string);
}  // namespace QueryUtility

bool QueryUtility::IsIdentifier(std::string const& string) {
  if (!std::isalpha(string[0])) {
    return false;
  }
  for (size_t i = 1; i < string.length(); i++) {
    if (!std::isalnum(string[i])) {
      return false;
    }
  }
  return true;
}

bool QueryUtility::IsQuotedIdentifier(std::string const& string) {
  if (string.size() >= 3) {
    // G8: magic number/string should be replaced by constants
    return (string.substr(0, 1) == "\"" &&
            IsSynonym(string.substr(1, string.size() - 2)) &&
            string.substr(string.size() - 1, 1) == "\"");
  }
  return false;
}

bool QueryUtility::IsSynonym(std::string const& string) {
  return QueryUtility::IsIdentifier(string);
}

bool QueryUtility::IsValidQPInteger(std::string const& string) {
  try {
    stoi(string);
    if (stoi(string) < 0) {
      return false;
    }
  } catch (std::invalid_argument& e) {
    return false;
  }
  if (string.front() == '0' && string.size() > 1) {
    return false;
  }
  return true;
}
