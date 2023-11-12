#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Clause.h"
#include "PatternClause.h"
#include "query_processing_system/references/EntRef.h"

class PatternAssignClause : public PatternClause {
 private:
  MatchType const rhs_expr_match_type;
  std::shared_ptr<TreeNode> rhs_expr;

  std::vector<std::pair<std::string, std::string>> EvaluateDeclRef(
      PKBQPSInterface& pkb) override;
  std::vector<std::string> EvaluateValueRef(PKBQPSInterface& pkb) override;
  std::vector<std::string> EvaluateWildRef(PKBQPSInterface& pkb) override;
  std::vector<std::pair<std::string, std::string>> EvaluateDeclRef(
      PKBQPSInterface& pkb, std::unordered_set<std::string>& decl_1_subset,
      std::unordered_set<std::string>& decl_2_subset) override;

 public:
  PatternAssignClause(PqlDeclaration assign_decl, EntRef lhs_ent_ref,
                      MatchType rhs_expr_match_type,
                      std::shared_ptr<TreeNode> rhs_expr);
};
