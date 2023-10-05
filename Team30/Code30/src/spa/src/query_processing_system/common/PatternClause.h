#pragma once

#include "Clause.h"
#include "EntRef.h"

class PatternClause : public Clause {
 private:
  const PqlDeclaration assign_decl;
  const EntRef lhs_ent_ref;
  const MatchType rhs_expr_match_type;
  const std::string rhs_expr;

 public:
  PatternClause(const PqlDeclaration& assign_decl, const EntRef& lhs_ent_ref,
                MatchType rhs_expr_match_type, std::string rhs_expr);
  std::unique_ptr<ClauseResult> evaluate(PkbApi& pkb) override;
};
