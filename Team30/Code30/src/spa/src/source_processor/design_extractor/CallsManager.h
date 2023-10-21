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
  void InsertCallsStmt(std::string proc_a, std::string proc_b,
                       std::vector<std::string> actors,
                       std::shared_ptr<CallNode> call_node);
  void InsertProcNode(std::string proc_name);
  void ExecuteCallsExtraction();
  void ConnectProcsAndUpdateRelations();

 private:
  // Key: string procedureName, Value: CallsGraphProcNode node
  std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
      proc_node_map;

  PKBSPInterface& pkb;
  std::shared_ptr<CallsExtractor> calls_extractor;
  std::shared_ptr<CallsProcConnector> proc_connector;
};
