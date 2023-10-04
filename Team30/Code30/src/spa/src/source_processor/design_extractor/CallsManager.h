#pragma once
#include <vector>
#include <memory>
#include <source_processor/node/stmt_node/CallNode.h>
#include <queue>
#include "CallsGraphProcNode.h"
#include <unordered_map>
#include "CallsExtractor.h"
#include "CallsProcConnector.h"

class CallsManager {
 public:
  explicit CallsManager(PkbApi& pkb);

  // procA: the procedure in which the call stmt is found
  // procB: the procedure that the call stmt calls
  // i.e. A calls B
  void insertCallsStmt(std::string procA, std::string procB,
                       std::vector<std::string> actors,
                       std::shared_ptr<CallNode> callNode);
  void insertProcNode(std::string procName);
  void executeCallsGraphTraversal();
  void executeCallsExtraction();
  void connectProcsAndUpdateRelations();

 private:
  // Key: string procedureName, Value: CallsGraphProcNode node
  std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>> procNodeMap;

  PkbApi& pkb;
  std::shared_ptr<CallsExtractor> callsExtractor;
  std::shared_ptr<CallsProcConnector> procConnector;
};