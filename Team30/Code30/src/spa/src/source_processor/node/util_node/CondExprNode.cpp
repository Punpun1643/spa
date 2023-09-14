#include "CondExprNode.h"

CondExprNode::CondExprNode(std::unordered_set<std::string> variables,
                           std::unordered_set<int> constants)
    : variables(std::move(variables)), constants(std::move(constants)) {}

std::unordered_set<std::string> CondExprNode::getVariables() const { return variables; }

std::unordered_set<int> CondExprNode::getConstants() const { return constants; }