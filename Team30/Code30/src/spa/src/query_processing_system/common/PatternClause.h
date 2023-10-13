#pragma once

#include "Clause.h"
#include "EntRef.h"

class PatternClause : public Clause {
 private:
  PqlDeclaration const assign_decl;
  EntRef const lhs_ent_ref;
  MatchType const rhs_expr_match_type;
  std::string const rhs_expr;  // to_delete
  std::shared_ptr<TreeNode> rhs_tree_expr;

 public:
  // legacy method
  PatternClause(PqlDeclaration const& assign_decl, EntRef const& lhs_ent_ref,
                MatchType rhs_expr_match_type, std::string rhs_expr);

  // new method
  PatternClause(PqlDeclaration assign_decl, EntRef lhs_ent_ref,
                MatchType rhs_expr_match_type,
                std::shared_ptr<TreeNode> rhs_expr);

  std::unique_ptr<ClauseResult> evaluate(PKBQPSInterface& pkb) override;
};
