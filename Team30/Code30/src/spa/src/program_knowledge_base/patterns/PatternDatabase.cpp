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

void PatternDatabase::InsertAssignment(std::string const& line_num,
                                       std::string const& lhs,
                                       std::shared_ptr<TreeNode> const& rhs) {
  assignments.insert({line_num, {lhs, rhs}});
  lhs_assignments[lhs].insert(line_num);
}

void PatternDatabase::InsertCondVar(EntityType type,
                                    std::string const& line_num,
                                    std::string const& var) {
  cond_var_patterns.at(type)[line_num].insert(var);
  inv_cond_var_patterns.at(type)[var].insert(line_num);
}

std::vector<std::string> PatternDatabase::GetMatchingAssignStmts(
    std::unordered_set<std::string>& assign_stmts,
    std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type) {
  std::unordered_set<std::string> output;
  // Wild Wild
  if (match_type == MatchType::WILD_MATCH) {
    output = assign_stmts;
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Wild Partial
    output = GetMatchingAssignWildPartial(rhs_expr);
  } else {  // Wild Exact
    output = GetMatchingAssignWildExact(rhs_expr);
  }

  return std::vector<std::string>(output.begin(), output.end());
}

std::unordered_set<std::string> PatternDatabase::GetMatchingAssignWildPartial(
    std::shared_ptr<TreeNode> const& rhs_expr) {
  std::unordered_set<std::string> output;
  for (auto& pair : assignments) {
    if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
      output.insert(pair.first);
    }
  }
  return output;
}

std::unordered_set<std::string> PatternDatabase::GetMatchingAssignWildExact(
    std::shared_ptr<TreeNode> const& rhs_expr) {
  std::unordered_set<std::string> output;
  for (auto& pair : assignments) {
    if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
      output.insert(pair.first);
    }
  }
  return output;
}

std::vector<std::string> PatternDatabase::GetMatchingAssignStmts(
    std::string const& lhs_value, std::shared_ptr<TreeNode> const& rhs_expr,
    MatchType match_type) {
  std::unordered_set<std::string> output;
  // String Wild
  if (match_type == MatchType::WILD_MATCH) {
    if (lhs_assignments.count(lhs_value) == 0) {
      return std::vector<std::string>();
    }
    output = lhs_assignments.at(lhs_value);
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // String Partial
    output = GetMatchingAssignStringPartial(lhs_value, rhs_expr);
  } else {  // String Exact
    output = GetMatchingAssignStringExact(lhs_value, rhs_expr);
  }

  return std::vector<std::string>(output.begin(), output.end());
}

std::unordered_set<std::string> PatternDatabase::GetMatchingAssignStringPartial(
    std::string const& lhs_value, std::shared_ptr<TreeNode> const& rhs_expr) {
  std::unordered_set<std::string> output;
  for (auto& pair : assignments) {
    if (pair.second.first == lhs_value &&
        TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
      output.insert(pair.first);
    }
  }
  return output;
}

std::unordered_set<std::string> PatternDatabase::GetMatchingAssignStringExact(
    std::string const& lhs_value, std::shared_ptr<TreeNode> const& rhs_expr) {
  std::unordered_set<std::string> output;
  for (auto& pair : assignments) {
    if (pair.second.first == lhs_value &&
        TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
      output.insert(pair.first);
    }
  }
  return output;
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetMatchingAssignStmtLhsVarPairs(
    std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type) {
  std::vector<std::pair<std::string, std::string>> output;

  // Synonym Wild
  if (match_type == MatchType::WILD_MATCH) {
    for (auto& pair : assignments) {
      output.push_back(make_pair(pair.first, pair.second.first));
    }
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Synonym Partial
    output = GetMatchingAssignSynonymPartial(rhs_expr);
  } else {  // Synonym Exact
    output = GetMatchingAssignSynonymExact(rhs_expr);
  }

  return std::vector<std::pair<std::string, std::string>>(output.begin(),
                                                          output.end());
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetMatchingAssignSynonymPartial(
    std::shared_ptr<TreeNode> const& rhs_expr) {
  std::vector<std::pair<std::string, std::string>> output;
  for (auto& pair : assignments) {
    if (TreeNode::IsSubTree(pair.second.second, rhs_expr)) {
      output.push_back(make_pair(pair.first, pair.second.first));
    }
  }
  return output;
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetMatchingAssignSynonymExact(
    std::shared_ptr<TreeNode> const& rhs_expr) {
  std::vector<std::pair<std::string, std::string>> output;
  for (auto& pair : assignments) {
    if (TreeNode::IsSameTree(pair.second.second, rhs_expr)) {
      output.push_back(make_pair(pair.first, pair.second.first));
    }
  }
  return output;
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
    EntityType container_stmt_type, std::string const& var_name) {
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
    for (auto const& vars : stmt_vars.second) {
      output.push_back(std::make_pair(stmt_vars.first, vars));
    }
  }
  return output;
}
