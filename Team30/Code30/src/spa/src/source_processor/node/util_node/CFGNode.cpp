#include "CFGNode.h"

#include <queue>

#include "../stmt_node/AssignNode.h"

CFGNode::CFGNode(std::shared_ptr<StmtNode> node, StmtType stmtType,
                 std::unordered_set<std::string> uses_vars,
                 std::unordered_set<std::string> modifies_vars)
    : node(node),
      stmtType(stmtType),
      uses_vars(uses_vars),
      modifies_vars(modifies_vars) {}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getOutgoingNodes() {
  return outgoingNodes;
}

std::vector<std::shared_ptr<CFGNode>> CFGNode::getIncomingNodes() {
  return incomingNodes;
}

std::shared_ptr<StmtNode> CFGNode::getNode() {
  return node;
}

StmtType CFGNode::getNodeType() {
  return node->GetStmtType();
}

std::unordered_set<std::string> CFGNode::getUsesVars() {
  return uses_vars;
}

std::unordered_set<std::string> CFGNode::getModifiesVars() {
  return modifies_vars;
}

StmtType CFGNode::getStmtType() {
  return stmtType;
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

bool CFGNode::IsAssignOrReadOutgoingNode(std::shared_ptr<CFGNode> node) {
  return node->getStmtType() == StmtType::ASSIGN_STMT ||
         node->getStmtType() == StmtType::READ_STMT;
}

bool CFGNode::IsCallOutgoingNode(std::shared_ptr<CFGNode> node) {
  return node->getStmtType() == StmtType::CALL_STMT;
}

bool CFGNode::HandleAssignOrReadOutgoingNode(
    std::shared_ptr<CFGNode> outgoingNode,
    std::string const& varModifiedInStartNode) {
  std::unordered_set<std::string> varsModifiedInOutgoingNode =
      outgoingNode->getModifiesVars();
  std::string varModifiedInOutgoingNode = *varsModifiedInOutgoingNode.begin();

  return varModifiedInStartNode != varModifiedInOutgoingNode;
}

bool CFGNode::HandleCallOutgoingNode(
    std::shared_ptr<CFGNode> outgoingNode,
    std::string const& varModifiedInStartNode) {
  std::unordered_set<std::string> varsModifiedInCallOutgoingNode =
      outgoingNode->getModifiesVars();

  return varsModifiedInCallOutgoingNode.find(varModifiedInStartNode) ==
         varsModifiedInCallOutgoingNode.end();
}

bool CFGNode::ValidateStartAndEndNodes(std::shared_ptr<CFGNode> startNode,
                                       std::shared_ptr<CFGNode> endNode) {
  return startNode->getStmtType() == StmtType::ASSIGN_STMT &&
         endNode->getStmtType() == StmtType::ASSIGN_STMT;
}

std::string CFGNode::GetVarModifiedInStartNode(
    std::shared_ptr<CFGNode> startNode) {
  std::unordered_set<std::string> varsModifiedInStartNode =
      startNode->getModifiesVars();
  return *varsModifiedInStartNode.begin();
}

std::unordered_set<std::string> CFGNode::GetVarUsedInEndNode(
    std::shared_ptr<CFGNode> endNode) {
  return endNode->getUsesVars();
}

bool CFGNode::ValidatePossibleAffectsRelationship(
    std::string varModifiedInStartNode,
    std::unordered_set<std::string> varsUsedInEndNode) {
  return varsUsedInEndNode.find(varModifiedInStartNode) !=
         varsUsedInEndNode.end();
}

bool CFGNode::HasAffectsPath(std::shared_ptr<CFGNode> startNode,
                             std::shared_ptr<CFGNode> endNode) {
  if (!ValidateStartAndEndNodes(startNode, endNode)) {
    return false;
  }
  std::string varModifiedInStartNode = GetVarModifiedInStartNode(startNode);
  std::unordered_set<std::string> varsUsedInEndNode =
      GetVarUsedInEndNode(endNode);

  if (!ValidatePossibleAffectsRelationship(varModifiedInStartNode,
                                           varsUsedInEndNode)) {
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
        bool shouldVisit = false;

        if (IsAssignOrReadOutgoingNode(outgoingNode)) {
          shouldVisit = HandleAssignOrReadOutgoingNode(outgoingNode,
                                                       varModifiedInStartNode);
        } else if (IsCallOutgoingNode(outgoingNode)) {
          shouldVisit =
              HandleCallOutgoingNode(outgoingNode, varModifiedInStartNode);
        } else {
          shouldVisit = true;
        }

        if (shouldVisit) {
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
