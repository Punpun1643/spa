#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

namespace ArrayUtility {
void RemoveDuplicates(std::vector<std::vector<std::string>>& vec);

std::vector<std::string> FlattenVector(
    std::vector<std::vector<std::string>>& vec);

std::pair<std::vector<std::string>, std::vector<std::string>> SplitPairVector(std::vector<std::pair<std::string, std::string>> const& vec);

}  // namespace ArrayUtility
