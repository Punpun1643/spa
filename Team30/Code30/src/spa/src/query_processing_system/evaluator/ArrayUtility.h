#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ArrayUtility {
void removeDuplicates(std::vector<std::vector<std::string>>& vec);

std::vector<std::string> flattenVector(
    std::vector<std::vector<std::string>>& vec);
}  // namespace ArrayUtility
