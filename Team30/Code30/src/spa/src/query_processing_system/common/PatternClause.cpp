#include "PatternClause.h"

#include <cassert>
#include <utility>

#include "query_processing_system/exceptions/InvalidSemanticsException.h"

PatternClause::PatternClause(PqlDeclaration assign_decl,
                             EntRef lhs_ent_ref,
                             MatchType rhs_expr_match_type,
                             std::shared_ptr<TreeNode> rhs_expr)
    : assign_decl(std::move(assign_decl)),
      lhs_ent_ref(std::move(lhs_ent_ref)),
      rhs_expr_match_type(rhs_expr_match_type),
      rhs_tree_expr(std::move(rhs_expr)) {
  if (this->assign_decl.getEntityType() != EntityType::ASSIGN) {
    throw InvalidSemanticsException(
        "Pattern Clause only accepts assign declarations");
  }

  if (this->lhs_ent_ref.getRefType() == PqlRefType::DECLARATION &&
      this->lhs_ent_ref.getDeclarationType() != EntityType::VARIABLE) {
    throw InvalidSemanticsException(
        "If the entRef of the pattern clause is a declaration, it must be of "
        "type variable.");
  }
}

// TO DELETE:
PatternClause::PatternClause(PqlDeclaration const& assign_decl,
                             EntRef const& lhs_ent_ref,
                             MatchType rhs_expr_match_type,
                             std::string rhs_expr)
    : assign_decl(assign_decl),
      lhs_ent_ref(lhs_ent_ref),
      rhs_expr_match_type(rhs_expr_match_type),
      rhs_expr(std::move(rhs_expr)) {
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
  if (rhs_tree_expr == nullptr) {
    switch (lhs_ent_ref.getRefType()) {
      case (PqlRefType::DECLARATION): {
        auto values = pkb.getPatternMatchesSynonymLhs(
            rhs_expr, rhs_expr_match_type);
        return std::make_unique<ClauseResult>(
            assign_decl, lhs_ent_ref.getDeclaration(), *values);
        break;
      }
      case (PqlRefType::VALUE): {
        auto values = pkb.getPatternMatchesValueLhs(
            lhs_ent_ref.getValue(), rhs_expr, rhs_expr_match_type);
        return std::make_unique<ClauseResult>(assign_decl, *values);
        break;
      }
      case (PqlRefType::WILD): {
        auto values =
            pkb.getPatternMatchesWildLhs(rhs_expr, rhs_expr_match_type);
        return std::make_unique<ClauseResult>(assign_decl, *values);
        break;
      }
      default:
        break;
    }
  } else {
    switch (lhs_ent_ref.getRefType()) {
      case (PqlRefType::DECLARATION): {
        auto values = pkb.getPatternMatchesSynonymLhs(
            rhs_tree_expr, rhs_expr_match_type);
        return std::make_unique<ClauseResult>(
            assign_decl, lhs_ent_ref.getDeclaration(), *values);
        break;
      }
      case (PqlRefType::VALUE): {
        auto values = pkb.getPatternMatchesValueLhs(
            lhs_ent_ref.getValue(), rhs_tree_expr, rhs_expr_match_type);
        return std::make_unique<ClauseResult>(assign_decl, *values);
        break;
      }
      case (PqlRefType::WILD): {
        auto values =
            pkb.getPatternMatchesWildLhs(rhs_tree_expr, rhs_expr_match_type);
        return std::make_unique<ClauseResult>(assign_decl, *values);
        break;
      }
      default:
        break;
    }
  }

}
