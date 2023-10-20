#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "../node/stmt_node/CallNode.h"
#include "../node/util_node/CallsGraphProcNode.h"
#include "CallsProcConnector.h"
#include "relation_extractor/CallsExtractor.h"

class CallsManager {
 public:
  explicit CallsManager(PKBSPInterface& pkb);

  // procA: the procedure in which the call stmt is found
  // procB: the procedure that the call stmt calls
  // i.e. A calls B
  void InsertCallsStmt(std::string procA, std::string procB,
                       std::vector<std::string> actors,
                       std::shared_ptr<CallNode> callNode);
  void InsertProcNode(std::string procName);
  void ExecuteCallsExtraction();
  void ConnectProcsAndUpdateRelations();

 private:
  // Key: string procedureName, Value: CallsGraphProcNode node
  std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
      procNodeMap;

  PKBSPInterface& pkb;
  std::shared_ptr<CallsExtractor> callsExtractor;
  std::shared_ptr<CallsProcConnector> procConnector;
};
