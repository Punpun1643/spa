#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace ArrayUtility {
  void removeDuplicates(std::vector<std::string> &vec);

  std::unique_ptr<std::vector<int>> getIndicesToKeep(const std::vector<std::string>& arr, const std::vector<std::string>& overlap_arr);
  std::unique_ptr<std::vector<std::string>> intersectLists(
    std::vector<std::string> arr_1, std::vector<std::string> arr_2);
  std::unique_ptr<std::vector<std::string>> getArrSubsetByIndices(const std::vector<std::string>& arr, const std::vector<int>& indices);
}

