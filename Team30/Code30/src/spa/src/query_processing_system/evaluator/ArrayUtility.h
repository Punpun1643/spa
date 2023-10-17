#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ArrayUtility {
void RemoveDuplicates(std::vector<std::vector<std::string>>& vec);

std::vector<std::string> FlattenVector(
    std::vector<std::vector<std::string>>& vec);
}  // namespace ArrayUtility
