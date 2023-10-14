#include "CFGNode.h"

#include <queue>
#include <unordered_set>

CFGNode::CFGNode(std::shared_ptr<StmtNode> node) : node(node) {}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getOutgoingNodes() {
  return outgoingNodes;
}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getIncomingNodes() {
  return incomingNodes;
}

std::shared_ptr<StmtNode> CFGNode::getNode() { return node; }

StmtType CFGNode::getNodeType() { return node->GetStmtType(); }

void CFGNode::addOutgoingNode(std::shared_ptr<CFGNode> newNode) {
  outgoingNodes.push_back(newNode);
}

void CFGNode::addIncomingNode(std::shared_ptr<CFGNode> newNode) {
  incomingNodes.push_back(newNode);
}

bool CFGNode::HasPath(std::shared_ptr<CFGNode> startNode,
                      std::shared_ptr<CFGNode> endNode) {
  //  if (startNode == endNode) {
  //    return true;
  //  }

  // visited nodes
  std::unordered_set<std::shared_ptr<CFGNode>> visitedNodes;
  // nodes to visit
  std::queue<std::shared_ptr<CFGNode>> nodesToVisit;

  nodesToVisit.push(startNode);

  while (!nodesToVisit.empty()) {
    std::shared_ptr<CFGNode> currNode = nodesToVisit.front();
    nodesToVisit.pop();
    visitedNodes.insert(currNode);

    if (currNode == endNode) {
      return true;
    }

    std::vector<std::shared_ptr<CFGNode>> outgoingNodes =
        currNode->getOutgoingNodes();

    for (std::shared_ptr<CFGNode> outgoingNode : outgoingNodes) {
      if (visitedNodes.find(outgoingNode) == visitedNodes.end()) {
        nodesToVisit.push(outgoingNode);
      }
    }
  }
  return false;
}

bool CFGNode::HasAffectPath(std::shared_ptr<CFGNode> startNode,
                            std::shared_ptr<CFGNode> endNode) {
  return true;
}
