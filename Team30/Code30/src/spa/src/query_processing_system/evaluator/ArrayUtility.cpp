#include "ArrayUtility.h"
#include <unordered_set>

void ArrayUtility::removeDuplicates(std::vector<std::string> &vec) {
  std::sort(vec.begin(), vec.end());
  vec.erase( std::unique( vec.begin(), vec.end() ), vec.end() );
}

std::unique_ptr<std::vector<int>> ArrayUtility::getIndicesToKeep(const std::vector<std::string> &arr,
                                                                             const std::vector<std::string>& overlap_arr) {
  /**
   * Given an arr and an overlap_arr, returns the list of indices in arr that should be kept
   * such that all values in arr are also present in overlap_arr.
   * Example: (['4','2','2','1'], ['2','4']) -> [0,1,2]
   */
  auto idx_to_keep = std::make_unique<std::vector<int>>();

  if (arr.empty() || overlap_arr.empty()) {
    return idx_to_keep; // empty
  }

  auto overlap_set = std::unordered_set<std::string>(overlap_arr.begin(), overlap_arr.end());

  for (auto i = 0; i < arr.size(); i++) {
    if (overlap_set.count(arr[i]) == 1) {
      idx_to_keep->push_back(i);
    }
  }
  return idx_to_keep;
}

std::unique_ptr<std::vector<std::string>> ArrayUtility::intersectLists(std::vector<std::string> arr_1, std::vector<std::string> arr_2) {
  // Sort the vectors to use std::set_intersection
  std::sort(arr_1.begin(), arr_1.end());
  std::sort(arr_2.begin(), arr_2.end());

  std::vector<std::string> intersecting_values;
  std::set_intersection(arr_1.begin(), arr_1.end(), arr_2.begin(), arr_2.end(),
                        std::back_inserter(intersecting_values));
  return std::make_unique<std::vector<std::string>>(intersecting_values);
}

std::unique_ptr<std::vector<std::string>> ArrayUtility::getArrSubsetByIndices(const std::vector<std::string> & arr, const std::vector<int> & indices) {
  auto result = std::make_unique<std::vector<std::string>>();
  for (auto idx: indices) {
    assert(0 <= idx && idx < arr.size());
    result->push_back(arr[idx]);
  }
  return result;
}
