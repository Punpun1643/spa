#include "ArrayUtility.h"

#include <unordered_set>

void ArrayUtility::removeDuplicates(std::vector<std::string>& vec) {
  /**
   * Removes duplicates from the given vector in-place.
   */
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

std::vector<std::string> ArrayUtility::intersectLists(
    std::vector<std::string> arr_1, std::vector<std::string> arr_2) {
  // Sort the vectors to use std::set_intersection
  std::sort(arr_1.begin(), arr_1.end());
  std::sort(arr_2.begin(), arr_2.end());

  std::vector<std::string> intersecting_values;
  std::set_intersection(arr_1.begin(), arr_1.end(), arr_2.begin(), arr_2.end(),
                        std::back_inserter(intersecting_values));
  return intersecting_values;
}
