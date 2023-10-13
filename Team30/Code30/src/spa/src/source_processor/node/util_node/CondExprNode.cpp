#include "CondExprNode.h"

CondExprNode::CondExprNode(std::unordered_set<std::string> vars,
                           std::unordered_set<int> consts)
    : variables(
          std::make_shared<std::unordered_set<std::string>>(std::move(vars))),
      constants(std::make_shared<std::unordered_set<int>>(std::move(consts))) {}

std::shared_ptr<std::unordered_set<std::string>> CondExprNode::GetVariables()
    const {
  return variables;
}

std::shared_ptr<std::unordered_set<int>> CondExprNode::GetConstants() const {
  return constants;
}