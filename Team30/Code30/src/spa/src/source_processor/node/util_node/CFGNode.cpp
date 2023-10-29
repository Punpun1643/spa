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

std::vector<std::shared_ptr<CFGNode>> CFGNode::GetIncomingNodes() {
  return incoming_nodes;
}

std::shared_ptr<StmtNode> CFGNode::GetNode() {
  return node;
}

StmtType CFGNode::GetNodeType() {
  return node->GetStmtType();
}

std::unordered_set<std::string> CFGNode::GetUsesVars() {
  return uses_vars;
}

std::unordered_set<std::string> CFGNode::GetModifiesVars() {
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

bool CFGNode::HasImmediatePath(std::shared_ptr<CFGNode> start_node,
                               std::shared_ptr<CFGNode> end_node) {
  std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
      start_node->GetOutgoingNodes();

  for (std::shared_ptr<CFGNode> outgoingNode : outgoing_nodes) {
    if (outgoingNode == end_node) {
      return true;
    }
  }
  return false;
}

bool CFGNode::HasPath(std::shared_ptr<CFGNode> start_node,
                      std::shared_ptr<CFGNode> end_node) {
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;

  nodes_to_visit.push(start_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();
    visited_nodes.insert(curr_node);

    if (curr_node == end_node && curr_node != start_node) {
      return true;
    }

    std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
        curr_node->GetOutgoingNodes();

    for (std::shared_ptr<CFGNode> out_going_node : outgoing_nodes) {
      if (visited_nodes.find(out_going_node) == visited_nodes.end()) {
        nodes_to_visit.push(out_going_node);
      } else if (out_going_node == end_node &&
                 visited_nodes.find(out_going_node) != visited_nodes.end()) {
        return true;
      }
    }
  }
  return false;
}

bool CFGNode::IsAssignOrReadOutgoingNode(std::shared_ptr<CFGNode> node) {
  return node->GetStmtType() == StmtType::ASSIGN_STMT ||
         node->GetStmtType() == StmtType::READ_STMT;
}

bool CFGNode::IsCallOutgoingNode(std::shared_ptr<CFGNode> node) {
  return node->GetStmtType() == StmtType::CALL_STMT;
}

bool CFGNode::HandleAssignOrReadOutgoingNode(
    std::shared_ptr<CFGNode> outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_outgoing_node =
      outgoing_node->GetModifiesVars();
  std::string var_modified_in_outgoing_node =
      *vars_modified_in_outgoing_node.begin();

  return var_modified_in_start_node != var_modified_in_outgoing_node;
}

bool CFGNode::HandleCallOutgoingNode(
    std::shared_ptr<CFGNode> outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_call_outgoing_node =
      outgoing_node->GetModifiesVars();

  return vars_modified_in_call_outgoing_node.find(var_modified_in_start_node) ==
         vars_modified_in_call_outgoing_node.end();
}

bool CFGNode::ValidateStartAndEndNodes(std::shared_ptr<CFGNode> start_node,
                                       std::shared_ptr<CFGNode> end_node) {
  return start_node->GetStmtType() == StmtType::ASSIGN_STMT &&
         end_node->GetStmtType() == StmtType::ASSIGN_STMT;
}

std::string CFGNode::GetVarModifiedInStartNode(
    std::shared_ptr<CFGNode> start_node) {
  std::unordered_set<std::string> vars_modified_in_start_node =
      start_node->GetModifiesVars();
  return *vars_modified_in_start_node.begin();
}

std::unordered_set<std::string> CFGNode::GetVarUsedInEndNode(
    std::shared_ptr<CFGNode> end_node) {
  return end_node->GetUsesVars();
}

bool CFGNode::ValidatePossibleAffectsRelationship(
    std::string var_modified_in_start_node,
    std::unordered_set<std::string> var_used_in_end_node) {
  return var_used_in_end_node.find(var_modified_in_start_node) !=
         var_used_in_end_node.end();
}

bool CFGNode::HasAffectsPath(std::shared_ptr<CFGNode> start_node,
                             std::shared_ptr<CFGNode> end_node) {
  if (!ValidateStartAndEndNodes(start_node, end_node)) {
    return false;
  }
  if (!ValidatePossibleAffectsRelationship(
          GetVarModifiedInStartNode(start_node),
          GetVarUsedInEndNode(end_node))) {
    return false;
  }
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  nodes_to_visit.push(start_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();
    visited_nodes.insert(curr_node);

    if (curr_node == end_node && curr_node != start_node) {
      return true;
    }
    for (std::shared_ptr<CFGNode> outgoing_node :
         curr_node->GetOutgoingNodes()) {
      if (outgoing_node == end_node) {
        return true;
      }

      if (!visited_nodes.count(outgoing_node)) {
        bool should_visit = true;
        if (IsAssignOrReadOutgoingNode(outgoing_node)) {
          should_visit = HandleAssignOrReadOutgoingNode(
              outgoing_node, GetVarModifiedInStartNode(start_node));
        } else if (IsCallOutgoingNode(outgoing_node)) {
          should_visit = HandleCallOutgoingNode(
              outgoing_node, GetVarModifiedInStartNode(start_node));
        }
        if (should_visit) {
          nodes_to_visit.push(outgoing_node);
        }
      } else if (outgoing_node == end_node &&
                 visited_nodes.count(outgoing_node)) {
        return true;
      }
    }
  }
  return false;
}
