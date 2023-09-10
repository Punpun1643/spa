#include "ClauseResult.h"

ClauseResult::ClauseResult() = default;

ClauseResult::ClauseResult(PqlDeclaration d, std::vector<std::string> values) {
  /* Create clause result with 1 declaration */
  value_map[d] = values;
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
    value_map[d1] = intersecting;
  } else {
    // separate out the paired vectors
    std::vector<std::string> v1;
    std::vector<std::string> v2;
    for (auto& pair : values) {
      v1.push_back(pair.first);
      v2.push_back(pair.second);
    }
    value_map[d1] = v1;
    value_map[d2] = v2;
    linkedDeclarations[d1] = {d2};
    linkedDeclarations[d2] = {d1};
  }
}

std::vector<std::string> ClauseResult::getIntersectingValues(
    std::vector<std::string>& arr_1, std::vector<std::string>& arr_2) {
  // Sort the vectors to use std::set_intersection
  std::sort(arr_1.begin(), arr_1.end());
  std::sort(arr_2.begin(), arr_2.end());

  std::vector<std::string> intersecting_values;
  std::set_intersection(arr_1.begin(), arr_1.end(), arr_2.begin(), arr_2.end(),
                        std::back_inserter(intersecting_values));
  return intersecting_values;
}

std::unique_ptr<std::vector<std::string>> ClauseResult::getValues(
    PqlDeclaration& declaration) const {
  if (value_map.count(declaration) == 0) {
    throw std::runtime_error("Given declaration does not exist.");
  }
  return std::make_unique<std::vector<std::string>>(value_map.at(declaration));
}

void ClauseResult::combineResults(ClauseResult& other) {
  /* For now, assume that at most 1 overlap. Can ignore linkages.*/
  std::unique_ptr<std::vector<PqlDeclaration>> common_declarations =
      getCommonDeclarations(other);
  if (common_declarations->empty()) {
    for (auto const& [key, value] : other.value_map) {
      value_map[key] = value;
    }
    for (auto const& [key, value] : other.linkedDeclarations) {
      linkedDeclarations[key] = value;
    }
  } else if (common_declarations->size() == 1) {
    PqlDeclaration shared_d = (*common_declarations)[0];
    value_map[shared_d] =
        getIntersectingValues(value_map[shared_d], other.value_map[shared_d]);
    // We ought to copy the rest, but for now it doesn't matter.
  }
}

std::unique_ptr<std::vector<PqlDeclaration>>
ClauseResult::getCommonDeclarations(ClauseResult& other) {
  std::unique_ptr<std::vector<PqlDeclaration>> common_declarations =
      std::make_unique<std::vector<PqlDeclaration>>();
  for (auto& it : value_map) {
    if (other.value_map.count(it.first) == 1) {
      common_declarations->push_back(it.first);
    }
  }
  return common_declarations;
}
