#pragma once

#include <string>
#include <memory>

#include "../../shared/parser/node/TreeNode.h"
#include "../interpreter/QueryInterpreter.h"
#include "PatternExpression.h"
#include "shared/types/MatchType.h"

class PatternAssignExpression : public PatternExpression {
 public:
  PatternAssignExpression(std::string synonym, std::string arg1,
                          std::string arg2, MatchType match_type,
                          std::shared_ptr<TreeNode> rhs_expr_tree, bool is_not)

  void acceptInterpreter(QueryInterpreter& interpreter) override;
  MatchType GetMatchType();
  std::shared_ptr<TreeNode> GetRhsExprTree();

  ~PatternAssignExpression() = default;

 private:
  MatchType match_type;
  std::shared_ptr<TreeNode> rhs_expr_tree;
};
