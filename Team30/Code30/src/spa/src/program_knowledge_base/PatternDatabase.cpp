#include "PatternDatabase.h"

#include <string>
#include <unordered_set>
#include <utility>

PatternDatabase::PatternDatabase() {}

void PatternDatabase::Insert(std::string line_num,
                             std::string lhs, std::shared_ptr<TreeNode> rhs) {
  assignments.insert({line_num, {lhs, rhs}});
  lhs_assignments[lhs].insert(line_num);
}

std::unordered_set<std::string> PatternDatabase::GetStatementNumbersGivenLHS(
    std::string v) {
  return lhs_assignments[v];
}

std::unordered_map<std::string,
                   std::pair<std::string, std::shared_ptr<TreeNode>>>
PatternDatabase::GetAssignmentPatterns() {
  return assignments;
}
