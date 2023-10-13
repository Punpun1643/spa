#include "PatternExpression.h"

PatternExpression::PatternExpression(std::string syn_assign, std::string arg1,
                                     std::string arg2, MatchType match_type,
                                     std::shared_ptr<TreeNode> rhs_expr_tree)
    : ClauseExpression(arg1, arg2),
      syn_assign(syn_assign),
      match_type(match_type),
      rhs_expr_tree(rhs_expr_tree){};

void PatternExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<PatternExpression>(shared_from_this()));
}

MatchType PatternExpression::GetMatchType() { return this->match_type; }
std::shared_ptr<TreeNode> PatternExpression::GetRhsExprTree() {
  return this->rhs_expr_tree;
}
std::string PatternExpression::GetSynAssign() { return this->syn_assign; }
