#include "CFGNode.h"

#include <queue>

#include "../stmt_node/AssignNode.h"

CFGNode::CFGNode(std::shared_ptr<StmtNode> node, StmtType stmt_type,
                 std::unordered_set<std::string> uses_vars,
                 std::unordered_set<std::string> modifies_vars)
    : node(node),
      stmt_type(stmt_type),
      uses_vars(uses_vars),
      modifies_vars(modifies_vars) {}

std::vector<std::shared_ptr<CFGNode>> CFGNode::GetOutgoingNodes() {
  return outgoing_nodes;
}

bool CFGNode::HasNoOutgoingNodes() {
  return outgoing_nodes.empty();
}

std::vector<std::shared_ptr<CFGNode>> CFGNode::GetIncomingNodes() {
  return incoming_nodes;
}

bool CFGNode::HasNoIncomingNodes() {
  return incoming_nodes.empty();
}

std::shared_ptr<StmtNode> CFGNode::GetNode() {
  return node;
}

StmtType CFGNode::GetNodeType() {
  return node->GetStmtType();
}

int CFGNode::GetNodeStmtIndex() {
  return node->GetStmtIndex();
}

std::unordered_set<std::string>& CFGNode::GetUsesVars() {
  return uses_vars;
}

std::unordered_set<std::string>& CFGNode::GetModifiesVars() {
  return modifies_vars;
}

StmtType CFGNode::GetStmtType() {
  return stmt_type;
}

bool CFGNode::UsesVar(std::string var) {
  return uses_vars.find(var) != uses_vars.end();
}
bool CFGNode::ModifiesVar(std::string var) {
  return modifies_vars.find(var) != modifies_vars.end();
}

void CFGNode::AddOutgoingNode(std::shared_ptr<CFGNode> new_node) {
  outgoing_nodes.push_back(new_node);
}

void CFGNode::AddIncomingNode(std::shared_ptr<CFGNode> new_node) {
  incoming_nodes.push_back(new_node);
}

bool CFGNode::IsAssignOrReadNode(std::shared_ptr<CFGNode> node) {
  return IsAssignNode(node) || IsReadNode(node);
}

bool CFGNode::IsAssignOrReadOrCallNode(std::shared_ptr<CFGNode> node) {
  return IsAssignNode(node) || IsReadNode(node) || IsCallNode(node);
}

bool CFGNode::IsReadNode(std::shared_ptr<CFGNode> node) {
  return node->GetStmtType() == StmtType::READ_STMT;
}

bool CFGNode::IsCallNode(std::shared_ptr<CFGNode> node) {
  return node->GetStmtType() == StmtType::CALL_STMT;
}

bool CFGNode::IsAssignNode(std::shared_ptr<CFGNode> node) {
  return node->GetStmtType() == StmtType::ASSIGN_STMT;
}

std::string CFGNode::GetVarModifiedInStartNode(
    std::shared_ptr<CFGNode> start_node) {
  std::unordered_set<std::string> vars_modified_in_start_node =
      start_node->GetModifiesVars();
  return *vars_modified_in_start_node.begin();
}

std::unordered_set<std::string>& CFGNode::GetVarUsedInEndNode(
    std::shared_ptr<CFGNode> end_node) {
  return end_node->GetUsesVars();
}
