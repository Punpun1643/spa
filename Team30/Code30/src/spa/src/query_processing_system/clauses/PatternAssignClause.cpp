#include "PatternAssignClause.h"

#include <memory>
#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

PatternAssignClause::PatternAssignClause(PqlDeclaration assign_decl,
                                         EntRef lhs_ent_ref,
                                         MatchType rhs_expr_match_type,
                                         std::shared_ptr<TreeNode> rhs_expr)
    : PatternClause(std::move(assign_decl), std::move(lhs_ent_ref),
                    EntityType::ASSIGN),
      rhs_expr_match_type(rhs_expr_match_type),
      rhs_expr(std::move(rhs_expr)) {}

std::vector<std::pair<std::string, std::string>>
PatternAssignClause::EvaluateDeclRef(PKBQPSInterface& pkb) {
  return pkb.GetMatchingAssignStmtLhsVarPairs(rhs_expr, rhs_expr_match_type);
}

std::vector<std::string> PatternAssignClause::EvaluateValueRef(
    PKBQPSInterface& pkb) {
  return pkb.GetMatchingAssignStmts(ent_ref.GetValue(), rhs_expr,
                                    rhs_expr_match_type);
}

std::vector<std::string> PatternAssignClause::EvaluateWildRef(
    PKBQPSInterface& pkb) {
  return pkb.GetMatchingAssignStmts(rhs_expr, rhs_expr_match_type);
}

std::vector<std::pair<std::string, std::string>>
PatternAssignClause::EvaluateDeclRef(
    PKBQPSInterface& pkb, std::unordered_set<std::string>& decl_1_subset,
    std::unordered_set<std::string>& decl_2_subset) {
  return pkb.GetMatchingAssignStmtLhsVarPairs(rhs_expr, rhs_expr_match_type,
                                              decl_1_subset, decl_2_subset);
}
