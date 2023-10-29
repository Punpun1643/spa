#include "GraphRelationTraverser.h"

#include <queue>

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
