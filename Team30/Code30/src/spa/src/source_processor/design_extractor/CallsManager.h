#pragma once
#include <vector>
#include <memory>
#include <source_processor/node/stmt_node/CallNode.h>
#include <queue>
#include "CallsGraphNode.h"

class CallsManager {
 public:
  explicit CallsManager();
  void insertCallsStmt(std::vector<std::string> actors,
                       std::shared_ptr<CallNode> callNode);
  void executeCallsGraphTraversal();

 private:
  std::priority_queue<CallsGraphNode> nodeQueue;
  std::vector<std::shared_ptr<CallNode>> existingNodes;
};