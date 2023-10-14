#pragma once

#include <memory>
#include <vector>

#include "../../../query_processing_system/common/EntityType.h"
#include "../util_node/CFGNode.h"
#include  "../stmt_node/StmtNode.h"
#include "../stmt_node/StmtType.h"

class CFGNode {
 public:
  CFGNode(std::shared_ptr<StmtNode> node);

  std::vector<std::shared_ptr<CFGNode>> getOutgoingNodes();

  std::vector<std::shared_ptr<CFGNode>> getIncomingNodes();

  std::shared_ptr<StmtNode> getNode();

  StmtType getNodeType();

  void addOutgoingNode(std::shared_ptr<CFGNode>);

  void addIncomingNode(std::shared_ptr<CFGNode>);

  static bool HasImmediatePath(std::shared_ptr<CFGNode> startNode, std::shared_ptr<CFGNode> endNode);

  static bool HasPath(std::shared_ptr<CFGNode> startNode,
                         std::shared_ptr<CFGNode> endNode);

  static bool HasAffectPath(std::shared_ptr<CFGNode> startNode,
                                 std::shared_ptr<CFGNode> endNode);

 private:
  std::vector<std::shared_ptr<CFGNode>> outgoingNodes;
  std::vector<std::shared_ptr<CFGNode>> incomingNodes;
  std::shared_ptr<StmtNode> node;
};