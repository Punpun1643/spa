#include "PatternClause.h"

#include <cassert>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

PatternClause::PatternClause(const PqlDeclaration& assign_decl,
                             const EntRef& lhs_ent_ref, MatchType rhs_expr_match_type,
                             std::string rhs_expr)
    : assign_decl(assign_decl),
      lhs_ent_ref(lhs_ent_ref),
      rhs_expr_match_type(rhs_expr_match_type),
      rhs_expr(std::move(rhs_expr)) {
  assert(rhs_expr_match_type != MatchType::EXACT_MATCH);  // not supported yet

  if (assign_decl.getEntityType() != EntityType::ASSIGN) {
    throw InvalidSemanticsException(
        "Pattern Clause only accepts assign declarations");
  }

  if (lhs_ent_ref.getRefType() == PqlRefType::DECLARATION &&
      lhs_ent_ref.getDeclarationType() != EntityType::VARIABLE) {
    throw InvalidSemanticsException(
        "If the entRef of the pattern clause is a declaration, it must be of "
        "type variable.");
  }
}

std::unique_ptr<ClauseResult> PatternClause::evaluate(PKBQPSInterface& pkb) {
  switch (lhs_ent_ref.getRefType()) {
    case (PqlRefType::DECLARATION): {
      auto values = pkb.getPatternMatchesWithDeclarationLhs(
          rhs_expr, rhs_expr_match_type);
      return std::make_unique<ClauseResult>(
          assign_decl, lhs_ent_ref.getDeclaration(), *values);
      break;
    }
    case (PqlRefType::VALUE): {
      auto values = pkb.getPatternMatchesWithLhsValue(
          lhs_ent_ref.getValue(), rhs_expr, rhs_expr_match_type);
      return std::make_unique<ClauseResult>(assign_decl, *values);
      break;
    }
    case (PqlRefType::WILD): {
      auto values =
          pkb.getPatternMatchesWithWildLhs(rhs_expr, rhs_expr_match_type);
      return std::make_unique<ClauseResult>(assign_decl, *values);
      break;
    }
    default:
      break;
  }
}
