#include "PatternDatabase.h"

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

PatternDatabase::PatternDatabase() {
  cond_var_patterns[EntityType::IF] =
      std::unordered_map<std::string, std::unordered_set<std::string>>();
  cond_var_patterns[EntityType::WHILE] =
      std::unordered_map<std::string, std::unordered_set<std::string>>();
  inv_cond_var_patterns[EntityType::IF] =
      std::unordered_map<std::string, std::unordered_set<std::string>>();
  inv_cond_var_patterns[EntityType::WHILE] =
      std::unordered_map<std::string, std::unordered_set<std::string>>();
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
    output = GetMatchingAssignPartial(rhs_expr, assign_stmts);
  } else {  // Wild Exact
    output = GetMatchingAssignExact(rhs_expr, assign_stmts);
  }
  return {output.begin(), output.end()};
}

std::unordered_set<std::string> PatternDatabase::GetMatchingAssignPartial(
    std::shared_ptr<TreeNode> const& rhs_expr,
    std::unordered_set<std::string> const& valid_assignments) {
  std::unordered_set<std::string> output;
  for (auto& stmt : valid_assignments) {
    if (TreeNode::IsSubTree(assignments.at(stmt).second, rhs_expr)) {
      output.insert(stmt);
    }
  }
  return output;
}

std::unordered_set<std::string> PatternDatabase::GetMatchingAssignExact(
    std::shared_ptr<TreeNode> const& rhs_expr,
    std::unordered_set<std::string> const& valid_assignments) {
  std::unordered_set<std::string> output;
  for (auto& stmt : valid_assignments) {
    if (TreeNode::IsSameTree(assignments.at(stmt).second, rhs_expr)) {
      output.insert(stmt);
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
      return {};
    }
    return {lhs_assignments.at(lhs_value).begin(),
            lhs_assignments.at(lhs_value).end()};
  } 
  
  std::unordered_set<std::string> valid_assignments =
      GetAllStmtsWithLHS(lhs_value);
  if (match_type == MatchType::PARTIAL_MATCH) {  // String Partial
    output = GetMatchingAssignPartial(rhs_expr, valid_assignments);
  } else {  // String Exact
    output = GetMatchingAssignExact(rhs_expr, valid_assignments);
  }

  return {output.begin(), output.end()};
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetMatchingAssignStmtLhsVarPairs(
    std::shared_ptr<TreeNode> const& rhs_expr, MatchType match_type,
    std::unordered_set<std::string> const& assign_syn_possible_values,
    std::unordered_set<std::string> const& var_syn_possible_values) {
  std::vector<std::pair<std::string, std::string>> output;

  // Synonym Wild
  if (match_type == MatchType::WILD_MATCH) {
    for (std::string const& stmt : assign_syn_possible_values) {
      output.emplace_back(stmt, assignments.at(stmt).first);
    }
  } else if (match_type == MatchType::PARTIAL_MATCH) {  // Synonym Partial
    std::unordered_set<std::string> valid_assignments =
        FilterAssignStmtsByStmtAndLHS(assign_syn_possible_values,
                                      var_syn_possible_values);
    output = GetMatchingAssignSynonymPartial(rhs_expr, valid_assignments);
  } else {  // Synonym Exact
    std::unordered_set<std::string> valid_assignments =
        FilterAssignStmtsByStmtAndLHS(assign_syn_possible_values,
                                      var_syn_possible_values);
    output = GetMatchingAssignSynonymExact(rhs_expr, valid_assignments);
  }

  return {output.begin(), output.end()};
}

std::unordered_set<std::string> PatternDatabase::FilterAssignStmtsByStmtAndLHS(
    std::unordered_set<std::string> const& assign_syn_possible_values,
    std::unordered_set<std::string> const& var_syn_possible_values) {
  std::unordered_set<std::string> valid_assignments;
  for (auto& stmt_lhsrhs : assignments) {
    if (assign_syn_possible_values.count(stmt_lhsrhs.first) &&
        var_syn_possible_values.count(stmt_lhsrhs.second.first)) {
      valid_assignments.insert(stmt_lhsrhs.first);
    }
  }
  return valid_assignments;
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetMatchingAssignSynonymPartial(
    std::shared_ptr<TreeNode> const& rhs_expr,
    std::unordered_set<std::string> const& valid_assignments) {
  std::vector<std::pair<std::string, std::string>> output;
  for (auto& stmt : valid_assignments) {
    std::pair<std::string, std::shared_ptr<TreeNode>> lhs_rhs =
        assignments.at(stmt);
    if (TreeNode::IsSubTree(lhs_rhs.second, rhs_expr)) {
      output.emplace_back(stmt, lhs_rhs.first);
    }
  }
  return output;
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetMatchingAssignSynonymExact(
    std::shared_ptr<TreeNode> const& rhs_expr,
    std::unordered_set<std::string> const& valid_assignments) {
  std::vector<std::pair<std::string, std::string>> output;
  for (auto& stmt : valid_assignments) {
    std::pair<std::string, std::shared_ptr<TreeNode>> lhs_rhs =
        assignments.at(stmt);
    if (TreeNode::IsSameTree(lhs_rhs.second, rhs_expr)) {
      output.emplace_back(stmt, lhs_rhs.first);
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
  return {output.begin(), output.end()};
}

std::vector<std::string> PatternDatabase::GetContainerStmtsWithGivenControlVar(
    EntityType container_stmt_type, std::string const& var_name) {
  if (inv_cond_var_patterns.at(container_stmt_type).count(var_name) == 0) {
    return {};
  }

  std::unordered_set<std::string> output =
      inv_cond_var_patterns.at(container_stmt_type).at(var_name);

  return {output.begin(), output.end()};
}

std::vector<std::pair<std::string, std::string>>
PatternDatabase::GetContainerStmtControlVarPairs(
    EntityType container_stmt_type,
    std::unordered_set<std::string> const& container_syn_possible_values,
    std::unordered_set<std::string> const& control_var_possible_values) {
  std::vector<std::pair<std::string, std::string>> output;
  std::unordered_map<std::string, std::unordered_set<std::string>>
      container_stmt_vars = cond_var_patterns.at(container_stmt_type);

  for (auto const& stmt : container_syn_possible_values) {
    // conditional does not have vars
    if (container_stmt_vars.count(stmt) == 0) {
      continue;
    }

    for (auto const& var : container_stmt_vars.at(stmt)) {
      if (control_var_possible_values.count(var) == 1) {
        output.emplace_back(stmt, var);
      }
    }
  }
  return output;
}

std::unordered_set<std::string> PatternDatabase::GetAllStmtsWithLHS(
    std::string const& lhs) {
  std::unordered_set<std::string> valid_assignments;
  for (auto& stmt_lhsrhs : assignments) {
    if (stmt_lhsrhs.second.first == lhs) {
      valid_assignments.insert(stmt_lhsrhs.first);
    }
  }
  return valid_assignments;
}
