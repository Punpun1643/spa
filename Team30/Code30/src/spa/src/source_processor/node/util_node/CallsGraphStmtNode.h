#pragma once

#include <memory>
#include <unordered_set>
#include <string>
#include <vector>

#include "../stmt_node/CallNode.h"

class CallsGraphStmtNode {
 public:
  CallsGraphStmtNode(std::vector<std::string> actors,
                     std::shared_ptr<CallNode> callNode);
  std::vector<std::string> getActors();
  std::shared_ptr<CallNode> getCallNode();

  ~CallsGraphStmtNode() = default;

 private:
  std::vector<std::string> actors;
  std::shared_ptr<CallNode> callNode;
};
