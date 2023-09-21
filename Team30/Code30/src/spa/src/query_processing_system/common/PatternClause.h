#pragma once

#include "Clause.h"
#include "EntRef.h"

class PatternClause : public Clause {
 private:
  std::shared_ptr<PqlDeclaration> assign_decl;
  EntRef lhs_ent_ref;
  MatchType rhs_expr_match_type;
  std::string rhs_expr;

 public:
  PatternClause(std::shared_ptr<PqlDeclaration> assign_decl,
                EntRef lhs_ent_ref, MatchType rhs_expr_match_type, std::string rhs_expr);
  std::unique_ptr<ClauseResult> evaluate(PkbApi& pkb) override;
};
