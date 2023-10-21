#pragma once

#include <memory>
#include <unordered_set>
#include <string>
#include <vector>

#include "../stmt_node/CallNode.h"

class CallsGraphStmtNode {
 public:
  CallsGraphStmtNode(std::vector<std::string> actors,
                     std::shared_ptr<CallNode> call_node);
  std::vector<std::string> GetActors();
  std::shared_ptr<CallNode> GetCallNode();

  ~CallsGraphStmtNode() = default;

 private:
  std::vector<std::string> actors;
  std::shared_ptr<CallNode> call_node;
};
