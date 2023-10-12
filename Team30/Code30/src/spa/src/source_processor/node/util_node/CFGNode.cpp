#include "CFGNode.h"

CFGNode::CFGNode(std::shared_ptr<StmtNode> node) : node(node) {}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getOutgoingNodes() {
  return outgoingNodes;
}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getIncomingNodes() {
  return incomingNodes;
}

std::shared_ptr<StmtNode> CFGNode::getNode() { return node; }

StmtType CFGNode::getNodeType() { return node->getStmtType(); }

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