#include "ArrayUtility.h"

#include <algorithm>
#include <unordered_set>

void ArrayUtility::RemoveDuplicates(
    std::vector<std::vector<std::string>>& vec) {
  /**
   * Removes duplicates from the given vector in-place.
   */
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

std::vector<std::string> ArrayUtility::FlattenVector(
    std::vector<std::vector<std::string>>& vec) {
  std::vector<std::string> output = {};
  for (auto const& row : vec) {
    for (auto const& value : row) {
      output.push_back(value);
    }
  }
  return output;
}
