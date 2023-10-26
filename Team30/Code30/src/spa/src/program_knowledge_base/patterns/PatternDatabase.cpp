#include "PatternDatabase.h"

#include <string>
#include <unordered_set>
#include <utility>

PatternDatabase::PatternDatabase() {}

void PatternDatabase::Insert(std::string line_num, std::string lhs,
                             std::shared_ptr<TreeNode> rhs) {
  assignments.insert({line_num, {lhs, rhs}});
  lhs_assignments[lhs].insert(line_num);
}

std::vector<std::string> PatternDatabase::GetMatchingAssignStmts(
    std::unordered_set<std::string> assign_stmts,
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  std::unordered_set<std::string> output;

  // Wild Wild
  if (match_type == MatchType::WILD_MATCH) {
    output = assign_stmts;
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Wild Partial
    for (auto& pair : assignments) {
      if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  } else {  // Wild Exact
    for (auto& pair : assignments) {
      if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  }

  return std::vector<std::string>(output.begin(), output.end());
}

std::vector<std::string> PatternDatabase::GetMatchingAssignStmts(
    std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
    MatchType match_type) {
  std::unordered_set<std::string> output;

  // String Wild
  if (match_type == MatchType::WILD_MATCH) {
    output = lhs_assignments[lhs_value];
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // String Partial
    for (auto& pair : assignments) {
      if (pair.second.first == lhs_value &&
          TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  } else {  // String Exact
    for (auto& pair : assignments) {
      if (pair.second.first == lhs_value &&
          TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.insert(pair.first);
      }
    }
  }

  return std::vector<std::string>(output.begin(), output.end());
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetMatchingAssignStmtLhsVarPairs(
    std::shared_ptr<TreeNode> rhs_expr, MatchType match_type) {
  std::vector<std::pair<std::string, std::string>> output;

  // Synonym Wild
  if (match_type == MatchType::WILD_MATCH) {
    for (auto& pair : assignments) {
      output.push_back(make_pair(pair.first, pair.second.first));
    }
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Synonym Partial
    for (auto& pair : assignments) {
      if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
        output.push_back(make_pair(pair.first, pair.second.first));
      }
    }
  } else {  // Synonym Exact
    for (auto& pair : assignments) {
      if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
        output.push_back(make_pair(pair.first, pair.second.first));
      }
    }
  }

  return std::vector<std::pair<std::string, std::string>>(output.begin(),
                                                          output.end());
}

std::vector<std::string> PatternDatabase::GetContainerStmtsWithControlVar(
    EntityType container_stmt_type) {
  return {};
}

std::vector<std::string> PatternDatabase::GetContainerStmtsWithGivenControlVar(
    EntityType container_stmt_type, std::string var_name) {
  return {};
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetContainerStmtControlVarPairs(
    EntityType container_stmt_type) {
  return {};
}
