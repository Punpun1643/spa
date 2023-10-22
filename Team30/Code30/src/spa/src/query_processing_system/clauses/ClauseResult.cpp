#include "ClauseResult.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <utility>

void ClauseResult::SetResultToFalse() {
  num_declarations = 0;
  boolean_clause_value = false;
}

ClauseResult::ClauseResult(bool is_valid)
    : num_declarations(0), boolean_clause_value(is_valid) {}

ClauseResult::ClauseResult(PqlDeclaration const& d,
                           std::vector<std::string> const& values) {
  if (values.empty()) {
    SetResultToFalse();
    return;
  }
  // Create clause result with 1 declaration
  num_declarations = 1;
  value_map[d] = values;
}

ClauseResult::ClauseResult(
    PqlDeclaration const& d1, PqlDeclaration const& d2,
    std::vector<std::pair<std::string, std::string>> const& values) {
  if (values.empty()) {
    SetResultToFalse();
    return;
  }

  /* Create a clause result with paired declarations */
  if (d1 == d2) {
    num_declarations = 1;
    std::vector<std::string> intersecting;
    for (auto& pair : values) {
      if (pair.first == pair.second) {
        intersecting.push_back(pair.first);
      }
    }
    if (intersecting.empty()) {
      SetResultToFalse();
    } else {
      value_map[d1] = intersecting;
    }
  } else {
    num_declarations = 2;
    // separate out the paired vectors
    std::vector<std::string> v1;
    std::vector<std::string> v2;
    for (auto& pair : values) {
      v1.push_back(pair.first);
      v2.push_back(pair.second);
    }
    value_map[d1] = v1;
    value_map[d2] = v2;
  }
}

int ClauseResult::GetNumDeclarations() const {
  return num_declarations;
}

bool ClauseResult::IsBooleanResult() const {
  return num_declarations == 0;
}

bool ClauseResult::GetBooleanClauseValue() const {
  assert(IsBooleanResult());
  return boolean_clause_value;
}

bool ClauseResult::Contains(PqlDeclaration const& d) const {
  return value_map.count(d) == 1;
}

std::vector<PqlDeclaration> ClauseResult::GetDeclarations() const {
  std::vector<PqlDeclaration> output = {};
  for (auto const& [key, value] : value_map)
    output.push_back(key);
  return output;
}

std::unique_ptr<std::vector<std::string>> ClauseResult::GetValues(
    PqlDeclaration const& declaration) const {
  if (value_map.count(declaration) == 0) {
    throw std::invalid_argument("Given declaration does not exist.");
  }
  return std::make_unique<std::vector<std::string>>(value_map.at(declaration));
}
