#include "ClauseResult.h"

#include <stdexcept>
#include <utility>

#include "shared/ArrayUtility.h"

void ClauseResult::SetResultToFalse() {
  num_declarations = 0;
  boolean_clause_value = false;
}

void ClauseResult::ConstructSingleDeclResult(
    PqlDeclaration d, std::vector<std::string> values) {
  if (values.empty()) {
    SetResultToFalse();
  } else {
    num_declarations = 1;
    d1 = std::move(d);
    d1_values = std::move(values);
  }
}

ClauseResult::ClauseResult(bool is_valid)
    : num_declarations(0), boolean_clause_value(is_valid) {}

ClauseResult::ClauseResult(PqlDeclaration d, std::vector<std::string> values) {
  ConstructSingleDeclResult(std::move(d), std::move(values));
}

ClauseResult::ClauseResult(
    PqlDeclaration d1, PqlDeclaration d2,
    std::vector<std::pair<std::string, std::string>> values) {
  if (values.empty()) {
    SetResultToFalse();
    return;
  }

  /* Create a clause result with paired declarations */
  if (d1 == d2) {
    std::vector<std::string> intersecting;
    for (auto& pair : values) {
      if (pair.first == pair.second) {
        intersecting.push_back(pair.first);
      }
    }
    ConstructSingleDeclResult(std::move(d1), std::move(intersecting));
  } else {
    num_declarations = 2;
    // separate out the paired vectors
    auto result = ArrayUtility::SplitPairVector(values);
    d1 = std::move(d1);
    d2 = std::move(d2);
    d1_values = std::move(result.first);
    d2_values = std::move(result.second);
  }
}

int ClauseResult::GetNumDeclarations() const {
  return num_declarations;
}

bool ClauseResult::IsBooleanResult() const {
  return num_declarations == 0;
}

bool ClauseResult::GetBooleanClauseValue() const {
  if (IsBooleanResult()) {
    return boolean_clause_value;
  } else {
    return true;  // contains values
  }
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

std::vector<std::string> ClauseResult::GetValues(
    PqlDeclaration const& declaration) const {
  if (value_map.count(declaration) == 0) {
    throw std::invalid_argument("Given declaration does not exist.");
  }
  return value_map.at(declaration);
}
