#include "GraphRelationTraverser.h"

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
