#include "ClauseResult.h"

ClauseResult::ClauseResult() = default;

ClauseResult::ClauseResult(PqlDeclaration d, std::vector<std::string> values) {
  value_map[d] = values;
}

ClauseResult::ClauseResult(PqlDeclaration d1, PqlDeclaration d2,
                           std::vector<std::pair<std::string, std::string>> values) {
  if (d1 == d2) {
    std::vector<std::string> intersecting_values;
    for (auto pair: values) {
      if (pair.first == pair.second) {
        intersecting_values.push_back(pair.first);
      }
    }
    value_map[d1] = intersecting_values;
  } else {
    // separate out the paired vectors
    std::vector<std::string> v1;
    std::vector<std::string> v2;
    for (auto pair: values) {
      v1.push_back(pair.first);
      v2.push_back(pair.second);
    }
    value_map[d1] = v1;
    value_map[d2] = v2;
    linkedDeclarations[d1] = {d2};
    linkedDeclarations[d2] = {d1};
  }
}

std::unique_ptr<std::vector<std::string>> ClauseResult::getIntersectingValues(
    std::vector<std::string>& arr_1, std::vector<std::string>& arr_2) {
  // Sort the vectors to use std::set_intersection
  std::sort(arr_1.begin(), arr_1.end());
  std::sort(arr_2.begin(), arr_2.end());

  std::unique_ptr<std::vector<std::string>> intersecting_values =
      std::make_unique<std::vector<std::string>>();
  std::set_intersection(arr_1.begin(), arr_1.end(), arr_2.begin(), arr_2.end(),
                        std::back_inserter(*intersecting_values));
  return intersecting_values;
}

std::unique_ptr<std::vector<std::string>> ClauseResult::getValues(
    PqlDeclaration declaration) {
  if (value_map.count(declaration) == 0) {
    throw std::runtime_error("Given declaration does not exist.");
  }
  std::unique_ptr<std::vector<std::string>> return_value =
      std::make_unique<std::vector<std::string>>(value_map[declaration]);
  return return_value;
}

std::unique_ptr<ClauseResult> ClauseResult::combineResults(
    ClauseResult& other) {
  // we need to be very careful...
  return std::make_unique<ClauseResult>();
}
