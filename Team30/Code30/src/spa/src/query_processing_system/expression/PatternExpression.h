#pragma once

#include "ClauseExpression.h"
#include "../common/MatchType.h"
#include "../interpreter/QueryInterpreter.h"
#include "../../shared/parser/node/TreeNode.h"

class PatternExpression : public ClauseExpression {
 public:
  PatternExpression(std::string syn_assign, std::string arg1, std::string arg2, MatchType match_type, std::shared_ptr<TreeNode> rhs_expr_tree);
  void acceptInterpreter(QueryInterpreter& interpreter) override;
  MatchType GetMatchType();
  std::shared_ptr<TreeNode> GetRhsExprTree();
  std::string GetSynAssign();

  ~PatternExpression() = default;

 private:
  MatchType match_type;
  std::string syn_assign;
  std::shared_ptr<TreeNode> rhs_expr_tree;
};
