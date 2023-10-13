#include "ArrayUtility.h"

#include <algorithm>
#include <unordered_set>

void ArrayUtility::removeDuplicates(
    std::vector<std::vector<std::string>>& vec) {
  /**
   * Removes duplicates from the given vector in-place.
   */
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

std::vector<std::string> ArrayUtility::flattenVector(
    std::vector<std::vector<std::string>>& vec) {
  std::vector<std::string> output = {};
  for (auto const& row : vec) {
    for (auto const& value : row) {
      output.push_back(value);
    }
  }
  return output;
}

std::string ArrayUtility::GetElemFromSingletonSet(std::unordered_set<std::string> const& singleton_set) {
  assert(singleton_set.size() == 1);
  for (auto elem: singleton_set) {
    return elem;
  }
  assert(false); // should not get here
}

std::vector<std::string> ArrayUtility::GetVectorFromSet(std::unordered_set<std::string> const& set) {
  std::vector<std::string> output = {};
  std::copy(set.begin(), set.end(), std::back_inserter(output));
  return output;
}
