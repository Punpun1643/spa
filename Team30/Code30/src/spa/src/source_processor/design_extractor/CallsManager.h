#pragma once
#include <vector>
#include <memory>
#include <source_processor/node/stmt_node/CallNode.h>
#include <queue>
#include "CallsGraphProcNode.h"
#include <unordered_map>

class CallsManager {
 public:
  explicit CallsManager();

  // procA: the procedure in which the call stmt is found
  // procB: the procedure that the call stmt calls
  // i.e. A calls B
  void insertCallsStmt(std::string procA, std::string procB,
                       std::vector<std::string> actors);
  void insertProcNode(std::string procName);
  void executeCallsGraphTraversal();

 private:
  // Key: string procedureName, Value: CallsGraphProcNode node
  std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>> procNodeMap;
};