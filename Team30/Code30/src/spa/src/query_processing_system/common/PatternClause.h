#pragma once

#include "Clause.h"
#include "EntRef.h"

class PatternClause : public Clause {
 private:
  PqlDeclaration const assign_decl;
  EntRef const lhs_ent_ref;
  MatchType const rhs_expr_match_type;
  std::string const rhs_expr;

 public:
  PatternClause(PqlDeclaration const& assign_decl, EntRef const& lhs_ent_ref,
                MatchType rhs_expr_match_type, std::string rhs_expr);
  std::unique_ptr<ClauseResult> evaluate(PKBQPSInterface& pkb) override;
};
