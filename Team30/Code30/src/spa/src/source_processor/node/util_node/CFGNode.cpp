#include "CFGNode.h"

CFGNode::CFGNode(std::shared_ptr<StmtNode> node,
                 std::unordered_set<std::string> uses_vars,
                 std::unordered_set<std::string> modifies_vars)
    : node(node), uses_vars(uses_vars), modifies_vars(modifies_vars) {}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getOutgoingNodes() {
  return outgoingNodes;
}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getIncomingNodes() {
  return incomingNodes;
}

std::shared_ptr<StmtNode> CFGNode::getNode() { return node; }

StmtType CFGNode::getNodeType() { return node->GetStmtType(); }

std::unordered_set<std::string> CFGNode::getUsesVars() { return uses_vars; }

std::unordered_set<std::string> CFGNode::getModifiesVars() {
  return modifies_vars;
}

bool CFGNode::UsesVar(std::string var) {
  return uses_vars.find(var) != uses_vars.end();
}
bool CFGNode::ModifiesVar(std::string var) {
  return modifies_vars.find(var) != modifies_vars.end();
}

void CFGNode::addOutgoingNode(std::shared_ptr<CFGNode> newNode) {
  outgoingNodes.push_back(newNode);
}

void CFGNode::addIncomingNode(std::shared_ptr<CFGNode> newNode) {
  incomingNodes.push_back(newNode);
}

bool CFGNode::HasPath(std::shared_ptr<CFGNode> startNode,
                      std::shared_ptr<CFGNode> endNode) {
  return true;
}

bool CFGNode::HasAffectPath(std::shared_ptr<CFGNode> startNode,
                            std::shared_ptr<CFGNode> endNode) {
  return true;
}