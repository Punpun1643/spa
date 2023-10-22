#pragma once

#include <memory>

#include "Clause.h"
#include "query_processing_system/references/EntRef.h"

class PatternClause : public Clause {
 private:
  PqlDeclaration const assign_decl;
  EntRef const lhs_ent_ref;
  MatchType const rhs_expr_match_type;
  std::shared_ptr<TreeNode> rhs_expr;

 public:
  PatternClause(PqlDeclaration assign_decl, EntRef lhs_ent_ref,
                MatchType rhs_expr_match_type,
                std::shared_ptr<TreeNode> rhs_expr);

  std::unique_ptr<ClauseResult> Evaluate(PKBQPSInterface& pkb) override;
};
