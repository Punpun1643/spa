#pragma once

#include <query_processing_system/common/EntityType.h>
#include <source_processor/node/stmt_node/StmtNode.h>

#include <memory>
#include <vector>

class CFGNode {
 public:
  CFGNode(std::shared_ptr<StmtNode> node);

  std::vector<std::shared_ptr<CFGNode>> getOutgoingNodes();
  std::vector<std::shared_ptr<CFGNode>> getIncomingNodes();
  std::shared_ptr<StmtNode> getNode();
  StmtType getNodeType();

  void addOutgoingNode(std::shared_ptr<CFGNode>);
  void addIncomingNode(std::shared_ptr<CFGNode>);

 private:
  std::vector<std::shared_ptr<CFGNode>> outgoingNodes;
  std::vector<std::shared_ptr<CFGNode>> incomingNodes;
  std::shared_ptr<StmtNode> node;
};