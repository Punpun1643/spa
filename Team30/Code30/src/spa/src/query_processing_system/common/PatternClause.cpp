#include "PatternClause.h"

#include <cassert>
#include <utility>
#include <memory>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

PatternClause::PatternClause(PqlDeclaration assign_decl, EntRef lhs_ent_ref,
                             MatchType rhs_expr_match_type,
                             std::shared_ptr<TreeNode> rhs_expr)
    : assign_decl(std::move(assign_decl)),
      lhs_ent_ref(std::move(lhs_ent_ref)),
      rhs_expr_match_type(rhs_expr_match_type),
      rhs_expr(std::move(rhs_expr)) {
  if (this->assign_decl.GetEntityType() != EntityType::ASSIGN) {
    throw InvalidSemanticsException(
        "Pattern Clause only accepts assign declarations");
  }

  if (this->lhs_ent_ref.GetRefType() == PqlRefType::DECLARATION &&
      this->lhs_ent_ref.GetDeclarationType() != EntityType::VARIABLE) {
    throw InvalidSemanticsException(
        "If the entRef of the pattern clause is a declaration, it must be of "
        "type variable.");
  }
}

std::unique_ptr<ClauseResult> PatternClause::Evaluate(PKBQPSInterface& pkb) {
  switch (lhs_ent_ref.GetRefType()) {
    case (PqlRefType::DECLARATION): {
      auto values =
          pkb.GetPatternMatchesSynonymLhs(rhs_expr, rhs_expr_match_type);
      return std::make_unique<ClauseResult>(
          assign_decl, lhs_ent_ref.GetDeclaration(), *values);
      break;
    }
    case (PqlRefType::VALUE): {
      auto values = pkb.GetPatternMatchesValueLhs(
          lhs_ent_ref.GetValue(), rhs_expr, rhs_expr_match_type);
      return std::make_unique<ClauseResult>(assign_decl, *values);
      break;
    }
    case (PqlRefType::WILD): {
      auto values = pkb.GetPatternMatchesWildLhs(rhs_expr, rhs_expr_match_type);
      return std::make_unique<ClauseResult>(assign_decl, *values);
      break;
    }
    default:
      break;
  }
}
