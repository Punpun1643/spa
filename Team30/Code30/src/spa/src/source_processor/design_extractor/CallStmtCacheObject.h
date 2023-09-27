#pragma once

#include <source_processor/node/stmt_node/CallNode.h>

#include <memory>
#include <unordered_set>

class CallStmtCacheObject {
 public:
  CallStmtCacheObject(std::vector<std::string> actors,
                      std::shared_ptr<CallNode> callNode);
  std::vector<std::string> getActors();
  std::shared_ptr<CallNode> getCallNode();
  bool updateVars(std::vector<std::string>> newVars);

  ~CallStmtCacheObject() = default;

 private:
  std::vector<std::string> actors;
  std::shared_ptr<CallNode> callNode;
  std::unordered_set<std::string> vars;
};