#include "ClauseResult.h"

#include <stdexcept>
#include <utility>

#include "shared/ArrayUtility.h"

ClauseResult::ClauseResult(bool is_valid)
    : num_declarations(0), boolean_clause_value(is_valid) {}

ClauseResult::ClauseResult(PqlDeclaration d, std::vector<std::string> values) {
  ConstructSingleDeclResult(std::move(d), std::move(values));
}

void ClauseResult::ConstructSingleDeclResult(PqlDeclaration d,
                                             std::vector<std::string> values) {
  num_declarations = 1;
  d1 = std::move(d);
  d1_values = std::move(values);
}

ClauseResult::ClauseResult(
    PqlDeclaration d1, PqlDeclaration d2,
    std::vector<std::pair<std::string, std::string>> values) {
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
    this->d1 = std::move(d1);
    this->d2 = std::move(d2);
    d1_d2_paired_values = std::move(values);
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
  if ((d1.has_value() && d1.value() == d) ||
      (d2.has_value() && d2.value() == d)) {
    return true;
  } else {
    return false;
  }
}

std::vector<PqlDeclaration> ClauseResult::GetDeclarations() const {
  std::vector<PqlDeclaration> output = {};
  if (d1.has_value()) {
    output.push_back(d1.value());
  }
  if (d2.has_value()) {
    output.push_back(d2.value());
  }
  return output;
}

std::vector<std::string> ClauseResult::GetSingleResultValues() {
  if (GetNumDeclarations() != 1) {
    throw std::logic_error("This ClauseResult does not have a single decl.");
  }
  return d1_values;
}

std::vector<std::pair<std::string, std::string>>
ClauseResult::GetPairedResultValues() {
  if (GetNumDeclarations() != 2) {
    throw std::logic_error("This ClauseResult does not have two decls.");
  }
  return d1_d2_paired_values;
}
