#include "ArrayUtility.h"

std::unique_ptr<std::unordered_map<std::string, int>> ArrayUtility::getValueCounts(const std::vector<std::string>& arr) {
  auto value_counts = std::make_unique<std::unordered_map<std::string, int>>();
  for (const std::string &value: arr) {
    if (value_counts->count(value) == 0) {
      (*value_counts)[value] = 1;
    } else {
      (*value_counts)[value] += 1;
    }
  }
  return value_counts;
}

std::unique_ptr<std::vector<int>> ArrayUtility::getIndicesToKeep(const std::vector<std::string> &arr,
                                                                             const std::vector<std::string>& overlap_arr) {
  /**
   * Given an arr and an overlap_arr, returns the list of indices in arr that should be kept
   * for all values in arr to also be present in overlap_arr. Takes into account duplicate
   * values.
   * Example: (['4','2','2','1'], ['2','2','4']) -> [0,1,2]
   */
  auto idx_to_keep = std::make_unique<std::vector<int>>();

  if (arr.empty() || overlap_arr.empty()) {
    return idx_to_keep; // empty
  }

  auto overlap_arr_value_counts = ArrayUtility::getValueCounts(overlap_arr);

  for (int i = 0; i < arr.size(); i++) {
    if (overlap_arr_value_counts->count(arr[i]) == 0 ||
        (*overlap_arr_value_counts)[arr[i]] <= 0) {
      continue;
    } else {
      (*overlap_arr_value_counts)[arr[i]] -= 1;
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
  for (int idx: indices) {
    assert(0 <= idx && idx < arr.size());
    result->push_back(arr[idx]);
  }
  return result;
}
