#include "ClauseResult.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

ClauseResult::ClauseResult(bool is_valid)
    : num_declarations(0), boolean_clause_value(is_valid) {}

ClauseResult::ClauseResult(PqlDeclaration d,
                           std::unique_ptr<std::vector<std::string>> values)
    : num_declarations(1) {
  /* Create clause result with 1 declaration */
  value_map[d] = *values;
}

ClauseResult::ClauseResult(
    PqlDeclaration d1, PqlDeclaration d2,
    std::unique_ptr<std::vector<std::pair<std::string, std::string>>> values) {
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
    linked_declarations[d1] = {d2};
    linked_declarations[d2] = {d1};
  }
}

int ClauseResult::getNumDeclarations() const { return num_declarations; }

bool ClauseResult::isBooleanResult() const { return num_declarations == 0; }

bool ClauseResult::getBooleanClauseValue() const {
  if (not isBooleanResult()) {
    throw std::runtime_error("ClauseResult obj has no boolean value.");
  } else {
    return boolean_clause_value;
  }
}

bool ClauseResult::contains(PqlDeclaration const& d) const {
  return value_map.count(d) == 1;
}

std::unique_ptr<std::vector<std::string>> ClauseResult::getValues(
    PqlDeclaration const& declaration) const {
  if (value_map.count(declaration) == 0) {
    throw std::runtime_error("Given declaration does not exist.");
  }
  return std::make_unique<std::vector<std::string>>(value_map.at(declaration));
}

//
// std::unique_ptr<ClauseResult> ClauseResult::combineResults(const
// ClauseResult& other) const{
//  if (num_declarations == 0) {
//    if (boolean_clause_value) {
//      return std::make_unique<ClauseResult>(other);
//    } else {
//      return std::make_unique<ClauseResult>(false);
//    }
//  } else if (other.num_declarations == 0) {
//    if (other.boolean_clause_value) {
//      return std::make_unique<ClauseResult>(this);
//    } else {
//      return std::make_unique<ClauseResult>(false);
//    }
//  }
//
//  /* For now, assume that at most 1 overlap. Can ignore linkages.*/
//  std::unique_ptr<std::vector<PqlDeclaration>> common_declarations =
//      getCommonDeclarations(other);
//  ClauseResult empty = ClauseResult();
//  auto output = std::make_unique<ClauseResult>(empty);
//
//  if (common_declarations->empty()) {
//    for (auto const& [key, value] : value_map) {
//      output->value_map[key] = value;
//    }
//    for (auto const& [key, value] : other.value_map) {
//      output->value_map[key] = value;
//    }
//    for (auto const& [key, value] : linked_declarations) {
//      output->linked_declarations[key] = value;
//    }
//    for (auto const& [key, value] : other.linked_declarations) {
//      output->linked_declarations[key] = value;
//    }
//  } else if (common_declarations->size() == 1) {
//    PqlDeclaration shared_d = (*common_declarations)[0];
//    output->value_map[shared_d] =
//        getIntersectingValues(value_map[shared_d], other.value_map[shared_d]);
//    // We ought to copy the rest, but for now it doesn't matter.
//  }
//}
//
// std::unique_ptr<std::vector<PqlDeclaration>>
// ClauseResult::getCommonDeclarations(const ClauseResult& other) const {
//  auto common_declarations =
//      std::make_unique<std::vector<PqlDeclaration>>();
//  for (auto& it : value_map) {
//    if (other.value_map.count(it.first) == 1) {
//      common_declarations->push_back(it.first);
//    }
//  }
//  return common_declarations;
//}
