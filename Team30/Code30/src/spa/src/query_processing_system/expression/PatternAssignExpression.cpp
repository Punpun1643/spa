#include "PatternAssignExpression.h"

#include <memory>
#include <string>

PatternAssignExpression::PatternAssignExpression(
    std::string synonym, std::string arg1, std::string arg2,
    MatchType match_type, std::shared_ptr<TreeNode> rhs_expr_tree)
    : PatternExpression(synonym, arg1, arg2),
      match_type(match_type),
      rhs_expr_tree(rhs_expr_tree){};

void PatternAssignExpression::acceptInterpreter(QueryInterpreter& interpreter) {
  interpreter.Interpret(
      std::dynamic_pointer_cast<PatternAssignExpression>(shared_from_this()));
}

MatchType PatternAssignExpression::GetMatchType() {
  return this->match_type;
}

std::shared_ptr<TreeNode> PatternAssignExpression::GetRhsExprTree() {
  return this->rhs_expr_tree;
}
