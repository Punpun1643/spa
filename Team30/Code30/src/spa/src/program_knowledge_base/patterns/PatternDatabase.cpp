#include "PatternDatabase.h"

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

PatternDatabase::PatternDatabase() {
  cond_var_patterns[EntityType::IF] = {};
  cond_var_patterns[EntityType::WHILE] = {};
  inv_cond_var_patterns[EntityType::IF] = {};
  inv_cond_var_patterns[EntityType::WHILE] = {};
}

void PatternDatabase::InsertAssignment(std::string line_num, std::string lhs,
                                       std::shared_ptr<TreeNode> rhs) {
  assignments.insert({line_num, {lhs, rhs}});
  lhs_assignments[lhs].insert(line_num);
}

void PatternDatabase::InsertCondVar(EntityType type, std::string line_num,
                                    std::string var) {
  cond_var_patterns.at(type)[line_num].insert(var);
  inv_cond_var_patterns.at(type)[var].insert(line_num);
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
    if (lhs_assignments.count(lhs_value) == 0) {
      return std::vector<std::string>();
    }
    output = lhs_assignments.at(lhs_value);
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
  std::unordered_set<std::string> output;

  for (auto const& stmt_vars : cond_var_patterns.at(container_stmt_type)) {
    if (!stmt_vars.second.empty()) {
      output.insert(stmt_vars.first);
    }
  }
  return std::vector<std::string>(output.begin(), output.end());
}

std::vector<std::string> PatternDatabase::GetContainerStmtsWithGivenControlVar(
    EntityType container_stmt_type, std::string var_name) {
  if (inv_cond_var_patterns.at(container_stmt_type).count(var_name) == 0) {
    return std::vector<std::string>();
  }

  std::unordered_set<std::string> output =
      inv_cond_var_patterns.at(container_stmt_type).at(var_name);

  return std::vector<std::string>(output.begin(), output.end());
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetContainerStmtControlVarPairs(
    EntityType container_stmt_type) {
  std::vector<std::pair<std::string, std::string>> output;
  for (auto const& stmt_vars : cond_var_patterns.at(container_stmt_type)) {
    for (auto vars : stmt_vars.second) {
      output.push_back(std::make_pair(stmt_vars.first, vars));
    }
  }
  return output;
}
