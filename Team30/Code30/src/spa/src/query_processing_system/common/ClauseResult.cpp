#include "ClauseResult.h"

ClauseResult::ClauseResult() = default;

void ClauseResult::insertData(PqlDeclaration declaration,
                              std::vector<std::string> possible_values) {
  if (results.count(declaration) == 0) {
    results[declaration] = possible_values;
  } else {
    std::vector<std::string> existing_values = results[declaration];
    results[declaration] =
        *getIntersectingValues(existing_values, possible_values);
  }
}

std::unique_ptr<std::vector<std::string>> ClauseResult::getIntersectingValues(
    std::vector<std::string>& arr_1,
    std::vector<std::string>& arr_2) {
  // Sort the vectors to use std::set_intersection
  std::sort(arr_1.begin(), arr_1.end());
  std::sort(arr_2.begin(), arr_2.end());

  std::unique_ptr<std::vector<std::string>> intersecting_values =
      std::make_unique<std::vector<std::string>>();
  std::set_intersection(arr_1.begin(),arr_1.end(),
                        arr_2.begin(),arr_2.end(),
                        std::back_inserter(*intersecting_values));
  return intersecting_values;
}

std::unique_ptr<ClauseResult> ClauseResult::combineResults(ClauseResult other) {
  return std::make_unique<ClauseResult>();
}
