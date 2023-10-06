#include "ArrayUtility.h"

#include <algorithm>
#include <unordered_set>

void ArrayUtility::removeDuplicates(std::vector<std::vector<std::string>>& vec) {
  /**
   * Removes duplicates from the given vector in-place.
   */
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

std::vector<std::string> ArrayUtility::flattenVector(std::vector<std::vector<std::string>>& vec) {
  std::vector<std::string> output = {};
  for (const auto& row: vec) {
    for (const auto& value: row) {
      output.push_back(value);
    }
  }
  return output;
}
