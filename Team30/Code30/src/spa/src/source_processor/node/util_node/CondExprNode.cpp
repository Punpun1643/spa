#include "CondExprNode.h"

CondExprNode::CondExprNode(std::unordered_set<std::string> vars,
                           std::unordered_set<int> consts) {
  variables = std::make_shared<std::unordered_set<std::string>>(vars);
  constants = std::make_shared<std::unordered_set<int>>(consts);
}

std::shared_ptr<std::unordered_set<std::string>> CondExprNode::getVariables()
    const {
  return variables;
}

std::shared_ptr<std::unordered_set<int>> CondExprNode::getConstants() const {
  return constants;
}