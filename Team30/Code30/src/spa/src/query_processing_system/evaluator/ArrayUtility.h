#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ArrayUtility {
void removeDuplicates(std::vector<std::string>& vec);
std::vector<std::string> intersectLists(std::vector<std::string> arr_1,
                                        std::vector<std::string> arr_2);
bool isContentEqual(std::vector<std::string> v1, std::vector<std::string> v2);
}  // namespace ArrayUtility