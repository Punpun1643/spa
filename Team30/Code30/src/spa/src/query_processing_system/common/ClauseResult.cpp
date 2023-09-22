#include "ClauseResult.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <stdexcept>

ClauseResult::ClauseResult(bool is_valid)
    : num_declarations(0), boolean_clause_value(is_valid) {}

ClauseResult::ClauseResult(PqlDeclaration d,
                           std::unique_ptr<std::vector<std::string>> values) {
  if (values->empty()) {
    // Entire clause becomes false because no possible values
    num_declarations = 0;
    boolean_clause_value = false;
    return;
  }
  // Create clause result with 1 declaration
  num_declarations = 1;
  value_map[d] = *values;
}

ClauseResult::ClauseResult(
    PqlDeclaration d1, PqlDeclaration d2,
    std::unique_ptr<std::vector<std::pair<std::string, std::string>>> values) {
  if (values->empty()) {
    // Entire clause becomes false because no possible values
    num_declarations = 0;
    boolean_clause_value = false;
    return;
  }

  /* Create a clause result with paired declarations */
  if (d1 == d2) {
    num_declarations = 1;
    std::vector<std::string> intersecting;
    for (auto& pair : *values) {
      if (pair.first == pair.second) {
        intersecting.push_back(pair.first);
      }
    }
    value_map[d1] = intersecting;
  } else {
    num_declarations = 2;
    // separate out the paired vectors
    std::vector<std::string> v1;
    std::vector<std::string> v2;
    for (auto& pair : *values) {
      v1.push_back(pair.first);
      v2.push_back(pair.second);
    }
    value_map[d1] = v1;
    value_map[d2] = v2;
  }
}

int ClauseResult::getNumDeclarations() const { return num_declarations; }

bool ClauseResult::isBooleanResult() const { return num_declarations == 0; }

bool ClauseResult::getBooleanClauseValue() const {
  assert(isBooleanResult());
  return boolean_clause_value;
}

bool ClauseResult::contains(PqlDeclaration const& d) const {
  return value_map.count(d) == 1;
}

std::vector<PqlDeclaration> ClauseResult::getDeclarations() const {
  std::vector<PqlDeclaration> output = {};
  for (auto const& [key, value] : value_map) output.push_back(key);
  return output;
}

std::unique_ptr<std::vector<std::string>> ClauseResult::getValues(
    PqlDeclaration const& declaration) const {
  if (value_map.count(declaration) == 0) {
    throw std::invalid_argument("Given declaration does not exist.");
  }
  return std::make_unique<std::vector<std::string>>(value_map.at(declaration));
}
