#include "CFGNode.h"

#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>

#include "../stmt_node/AssignNode.h"
#include "../stmt_node/CallNode.h"
#include "../stmt_node/ReadNode.h"

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

bool CFGNode::HasImmediatePath(std::shared_ptr<CFGNode> startNode,
                               std::shared_ptr<CFGNode> endNode) {
  std::vector<std::shared_ptr<CFGNode>> outgoingNodes =
      startNode->getOutgoingNodes();

  for (std::shared_ptr<CFGNode> outgoingNode : outgoingNodes) {
    if (outgoingNode == endNode) {
      return true;
    }
  }
  return false;
}

bool CFGNode::HasPath(std::shared_ptr<CFGNode> startNode,
                      std::shared_ptr<CFGNode> endNode) {
  std::unordered_set<std::shared_ptr<CFGNode>> visitedNodes;
  std::queue<std::shared_ptr<CFGNode>> nodesToVisit;

  nodesToVisit.push(startNode);

  while (!nodesToVisit.empty()) {
    std::shared_ptr<CFGNode> currNode = nodesToVisit.front();
    nodesToVisit.pop();
    visitedNodes.insert(currNode);

    if (currNode == endNode && currNode != startNode) {
      return true;
    }

    std::vector<std::shared_ptr<CFGNode>> outgoingNodes =
        currNode->getOutgoingNodes();

    for (std::shared_ptr<CFGNode> outgoingNode : outgoingNodes) {
      if (visitedNodes.find(outgoingNode) == visitedNodes.end()) {
        nodesToVisit.push(outgoingNode);
      } else if (outgoingNode == endNode &&
                 visitedNodes.find(outgoingNode) != visitedNodes.end()) {
        return true;
      }
    }
  }
  return false;
}

bool CFGNode::HasAffectsPath(std::shared_ptr<CFGNode> startNode,
                             std::shared_ptr<CFGNode> endNode) {
  if (startNode->getNode()->GetStmtType() != StmtType::ASSIGN_STMT ||
      endNode->getNode()->GetStmtType() != StmtType::ASSIGN_STMT) {
    return false;
  }

  std::unordered_set<std::string> varsModifiedInStartNode =
      startNode->getModifiesVars();
  std::string varModifiedInStartNode = *varsModifiedInStartNode.begin();

  std::unordered_set<std::string> varUsedInEndNode = endNode->getUsesVars();

  if (varUsedInEndNode.find(varModifiedInStartNode) == varUsedInEndNode.end()) {
    return false;
  }

  std::queue<std::shared_ptr<CFGNode>> nodesToVisit;
  std::unordered_set<std::shared_ptr<CFGNode>> visitedNodes;

  nodesToVisit.push(startNode);

  while (!nodesToVisit.empty()) {
    std::shared_ptr<CFGNode> currNode = nodesToVisit.front();
    nodesToVisit.pop();
    visitedNodes.insert(currNode);

    if (currNode == endNode && currNode != startNode) {
      return true;
    }

    std::vector<std::shared_ptr<CFGNode>> outgoingNodes =
        currNode->getOutgoingNodes();

    for (std::shared_ptr<CFGNode> outgoingNode : outgoingNodes) {
      if (outgoingNode == endNode) {
        return true;
      }

      if (visitedNodes.find(outgoingNode) == visitedNodes.end()) {
        if (outgoingNode->getNode()->GetStmtType() == StmtType::ASSIGN_STMT) {
          std::shared_ptr<AssignNode> assignOutgoingNode =
              std::dynamic_pointer_cast<AssignNode>(outgoingNode->getNode());
          std::string varModifiedInAssignOutgoingNode =
              assignOutgoingNode->GetVarName();

          if (varModifiedInStartNode != varModifiedInAssignOutgoingNode) {
            nodesToVisit.push(outgoingNode);
          }
        } else if (outgoingNode->getNode()->GetStmtType() ==
                   StmtType::READ_STMT) {
          std::shared_ptr<ReadNode> readOutgoingNode =
              std::dynamic_pointer_cast<ReadNode>(outgoingNode->getNode());
          std::string varModifiedInReadOutgoingNode =
              readOutgoingNode->GetVarName();

          if (varModifiedInStartNode != varModifiedInReadOutgoingNode) {
            nodesToVisit.push(outgoingNode);
          }
        } else if (outgoingNode->getNode()->GetStmtType() ==
                   StmtType::CALL_STMT) {
          std::shared_ptr<CallNode> callOutgoingNode =
              std::dynamic_pointer_cast<CallNode>(outgoingNode->getNode());

          // for now, testing
          nodesToVisit.push(outgoingNode);
        } else {
          nodesToVisit.push(outgoingNode);
        }
      } else if (outgoingNode == endNode &&
                 visitedNodes.find(outgoingNode) != visitedNodes.end()) {
        return true;
      }
    }
  }
  return false;
}
