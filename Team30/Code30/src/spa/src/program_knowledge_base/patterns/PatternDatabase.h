#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../../shared/parser/node/TreeNode.h"
#include "shared/types/EntityType.h"
#include "shared/types/MatchType.h"

class PatternDatabase {
 private:
  // {statement_number, (lhs, rhs)}
  std::unordered_map<std::string,
                     std::pair<std::string, std::shared_ptr<TreeNode>>>
      assignments;

  // {lhs variable : statement_number}, used for optimisation
  std::unordered_map<std::string, std::unordered_set<std::string>>
      lhs_assignments = {};

 public:
  PatternDatabase();
  ~PatternDatabase() = default;

  void Insert(std::string line_num, std::string lhs,
              std::shared_ptr<TreeNode> rhs);

  // Assign Pattern
  std::vector<std::string> GetMatchingAssignStmts(
      std::unordered_set<std::string> assign_stmts,
      std::shared_ptr<TreeNode> rhs_expr, MatchType match_type);
  std::vector<std::string> GetMatchingAssignStmts(
      std::string lhs_value, std::shared_ptr<TreeNode> rhs_expr,
      MatchType match_type);
  std::vector<std::pair<std::string, std::string>>
  GetMatchingAssignStmtLhsVarPairs(std::shared_ptr<TreeNode> rhs_expr,
                                   MatchType match_type);

  // If/While Patterns
  std::vector<std::string> GetContainerStmtsWithControlVar(
      EntityType container_stmt_type);
  std::vector<std::string> GetContainerStmtsWithGivenControlVar(
      EntityType container_stmt_type, std::string var_name);
  std::vector<std::pair<std::string, std::string>>
  GetContainerStmtControlVarPairs(EntityType container_stmt_type);
};
