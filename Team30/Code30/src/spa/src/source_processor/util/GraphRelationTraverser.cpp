#include "GraphRelationTraverser.h"

#include <queue>
#include <vector>

bool GraphRelationTraverser::HasImmediatePath(
    std::shared_ptr<CFGNode> start_node, std::shared_ptr<CFGNode> end_node) {
  std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
      start_node->GetOutgoingNodes();

  for (std::shared_ptr<CFGNode> outgoingNode : outgoing_nodes) {
    if (outgoingNode == end_node) {
      return true;
    }
  }
  return false;
}

bool GraphRelationTraverser::HasPath(std::shared_ptr<CFGNode> start_node,
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
      if (visited_nodes.count(out_going_node) == 0) {
        nodes_to_visit.push(out_going_node);
      } else if (out_going_node == end_node &&
                 visited_nodes.count(out_going_node) > 0) {
        return true;
      }
    }
  }
  return false;
}

bool GraphRelationTraverser::ValidateStartAndEndNodes(
    std::shared_ptr<CFGNode> start_node, std::shared_ptr<CFGNode> end_node) {
  return start_node->GetStmtType() == StmtType::ASSIGN_STMT &&
         end_node->GetStmtType() == StmtType::ASSIGN_STMT;
}

bool GraphRelationTraverser::ValidatePossibleAffectsRelationship(
    std::string var_modified_in_start_node,
    std::unordered_set<std::string> var_used_in_end_node) {
  return var_used_in_end_node.find(var_modified_in_start_node) !=
         var_used_in_end_node.end();
}

bool GraphRelationTraverser::HandleAssignOrReadOutgoingNode(
    std::shared_ptr<CFGNode> outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_outgoing_node =
      outgoing_node->GetModifiesVars();
  std::string var_modified_in_outgoing_node =
      *vars_modified_in_outgoing_node.begin();

  return var_modified_in_start_node != var_modified_in_outgoing_node;
}

bool GraphRelationTraverser::HandleCallOutgoingNode(
    std::shared_ptr<CFGNode> outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_call_outgoing_node =
      outgoing_node->GetModifiesVars();

  return vars_modified_in_call_outgoing_node.find(var_modified_in_start_node) ==
         vars_modified_in_call_outgoing_node.end();
}

bool GraphRelationTraverser::HasAffectsPath(std::shared_ptr<CFGNode> start_node,
                                            std::shared_ptr<CFGNode> end_node) {
  if (!ValidateStartAndEndNodes(start_node, end_node)) {
    return false;
  }
  if (!ValidatePossibleAffectsRelationship(
          CFGNode::GetVarModifiedInStartNode(start_node),
          CFGNode::GetVarUsedInEndNode(end_node))) {
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
        if (CFGNode::IsAssignOrReadOutgoingNode(outgoing_node)) {
          should_visit = HandleAssignOrReadOutgoingNode(
              outgoing_node, CFGNode::GetVarModifiedInStartNode(start_node));
        } else if (CFGNode::IsCallOutgoingNode(outgoing_node)) {
          should_visit = HandleCallOutgoingNode(
              outgoing_node, CFGNode::GetVarModifiedInStartNode(start_node));
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
