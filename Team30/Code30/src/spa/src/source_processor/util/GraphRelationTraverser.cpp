#include "GraphRelationTraverser.h"

#include <iostream>
#include <queue>
#include <vector>

bool GraphRelationTraverser::HasImmediatePath(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node) {
  std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
      start_node->GetOutgoingNodes();

  for (std::shared_ptr<CFGNode> outgoingNode : outgoing_nodes) {
    if (outgoingNode == end_node) {
      return true;
    }
  }
  return false;
}

bool GraphRelationTraverser::HasPath(std::shared_ptr<CFGNode> const& start_node,
                                     std::shared_ptr<CFGNode> const& end_node) {
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
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node) {
  return start_node->GetStmtType() == StmtType::ASSIGN_STMT &&
         end_node->GetStmtType() == StmtType::ASSIGN_STMT;
}

bool GraphRelationTraverser::ValidatePossibleAffectsRelationship(
    std::string var_modified_in_start_node,
    std::unordered_set<std::string>& var_used_in_end_node) {
  return var_used_in_end_node.find(var_modified_in_start_node) !=
         var_used_in_end_node.end();
}

bool GraphRelationTraverser::HandleAssignOrReadOutgoingNode(
    std::shared_ptr<CFGNode> const& outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_outgoing_node =
      outgoing_node->GetModifiesVars();
  std::string var_modified_in_outgoing_node =
      *vars_modified_in_outgoing_node.begin();

  return var_modified_in_start_node != var_modified_in_outgoing_node;
}

bool GraphRelationTraverser::HandleCallOutgoingNode(
    std::shared_ptr<CFGNode> const& outgoing_node,
    std::string const& var_modified_in_start_node) {
  std::unordered_set<std::string> vars_modified_in_call_outgoing_node =
      outgoing_node->GetModifiesVars();

  return vars_modified_in_call_outgoing_node.find(var_modified_in_start_node) ==
         vars_modified_in_call_outgoing_node.end();
}

bool GraphRelationTraverser::HasAffectsPath(
    std::shared_ptr<CFGNode> const& start_node,
    std::shared_ptr<CFGNode> const& end_node) {
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

bool GraphRelationTraverser::HasAnyAffectsPath(
    std::shared_ptr<CFGNode> const& start_node) {
  if (start_node->GetNodeType() != StmtType::ASSIGN_STMT) {
    return false;
  }

  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::string var_modified_in_start_node =
      CFGNode::GetVarModifiedInStartNode(start_node);

  nodes_to_visit.push(start_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();

    for (std::shared_ptr<CFGNode> outgoing_node :
         curr_node->GetOutgoingNodes()) {
      if (!visited_nodes.count(outgoing_node)) {
        visited_nodes.insert(outgoing_node);

        // if start node affects any outgoing node, return true
        if (ValidatePossibleAffectsRelationship(
                var_modified_in_start_node,
                CFGNode::GetVarUsedInEndNode(outgoing_node)) &&
            outgoing_node->GetNodeType() == StmtType::ASSIGN_STMT) {
          return true;
        }

        if (ShouldVisit(outgoing_node, var_modified_in_start_node)) {
          nodes_to_visit.push(outgoing_node);
        }
      }
    }
  }
  return false;
}

bool GraphRelationTraverser::HasAnyAffectsPathTo(
    std::shared_ptr<CFGNode> const& end_node) {
  if (end_node->GetNodeType() != StmtType::ASSIGN_STMT) {
    return false;
  }

  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::unordered_set<std::string> vars_used_in_end_node =
      CFGNode::GetVarUsedInEndNode(end_node);

  nodes_to_visit.push(end_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();

    for (std::shared_ptr<CFGNode> incoming_node :
         curr_node->GetIncomingNodes()) {
      if (!visited_nodes.count(incoming_node)) {
        visited_nodes.insert(incoming_node);

        // if incoming node changes a variable used by the end node, return true
        if (incoming_node->GetNodeType() == StmtType::ASSIGN_STMT &&
            ValidatePossibleAffectsRelationship(
                CFGNode::GetVarModifiedInStartNode(incoming_node),
                vars_used_in_end_node)) {
          return true;
        }
        nodes_to_visit.push(incoming_node);
      }
    }
  }
  return false;
}

std::unordered_set<std::string> GraphRelationTraverser::GetAllStmtsWithPathFrom(
    std::shared_ptr<CFGNode> const& start_node) {
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::string> stmts_with_valid_path;

  nodes_to_visit.push(start_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();

    std::vector<std::shared_ptr<CFGNode>> outgoing_nodes =
        curr_node->GetOutgoingNodes();

    for (std::shared_ptr<CFGNode> out_going_node : outgoing_nodes) {
      if (visited_nodes.find(out_going_node) == visited_nodes.end()) {
        nodes_to_visit.push(out_going_node);
        visited_nodes.insert(out_going_node);
        stmts_with_valid_path.insert(
            std::to_string(out_going_node->GetNode()->GetStmtIndex()));
      }
    }
  }
  return stmts_with_valid_path;
}

std::unordered_set<std::string> GraphRelationTraverser::GetAllStmtsWithPathTo(
    std::shared_ptr<CFGNode> const& end_node) {
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::string> stmts_with_valid_path;

  nodes_to_visit.push(end_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();

    std::vector<std::shared_ptr<CFGNode>> incoming_nodes =
        curr_node->GetIncomingNodes();

    for (std::shared_ptr<CFGNode> in_coming_node : incoming_nodes) {
      if (visited_nodes.find(in_coming_node) == visited_nodes.end()) {
        nodes_to_visit.push(in_coming_node);
        visited_nodes.insert(in_coming_node);
        stmts_with_valid_path.insert(
            std::to_string(in_coming_node->GetNode()->GetStmtIndex()));
      }
    }
  }
  return stmts_with_valid_path;
}

std::unordered_set<std::string>
GraphRelationTraverser::GetAllStmtsWithAffectsPathFrom(
    std::shared_ptr<CFGNode> const& start_node) {
  if (start_node->GetNodeType() != StmtType::ASSIGN_STMT) {
    return std::unordered_set<std::string>();
  }

  std::queue<std::shared_ptr<CFGNode>> nodes_to_visit;
  std::unordered_set<std::shared_ptr<CFGNode>> visited_nodes;
  std::unordered_set<std::string> stmts_with_valid_path;
  std::string var_modified_in_start_node =
      CFGNode::GetVarModifiedInStartNode(start_node);

  nodes_to_visit.push(start_node);

  while (!nodes_to_visit.empty()) {
    std::shared_ptr<CFGNode> curr_node = nodes_to_visit.front();
    nodes_to_visit.pop();

    for (std::shared_ptr<CFGNode> outgoing_node :
         curr_node->GetOutgoingNodes()) {
      if (!visited_nodes.count(outgoing_node)) {
        visited_nodes.insert(outgoing_node);

        // if start node affects outgoing node, add to stmts_with_valid_path
        if (ValidatePossibleAffectsRelationship(
                var_modified_in_start_node,
                CFGNode::GetVarUsedInEndNode(outgoing_node)) &&
            outgoing_node->GetNodeType() == StmtType::ASSIGN_STMT) {
          stmts_with_valid_path.insert(
              std::to_string(outgoing_node->GetNode()->GetStmtIndex()));
        }

        if (ShouldVisit(outgoing_node, var_modified_in_start_node)) {
          nodes_to_visit.push(outgoing_node);
        }
      }
    }
  }
  return stmts_with_valid_path;
}

bool GraphRelationTraverser::ShouldVisit(
    std::shared_ptr<CFGNode> const& node,
    std ::string var_modified_in_start_node) {
  bool should_visit = true;
  if (CFGNode::IsAssignOrReadOutgoingNode(node)) {
    should_visit =
        HandleAssignOrReadOutgoingNode(node, var_modified_in_start_node);
  } else if (CFGNode::IsCallOutgoingNode(node)) {
    should_visit = HandleCallOutgoingNode(node, var_modified_in_start_node);
  }
  return should_visit;
}
