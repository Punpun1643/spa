#include "CallsManager.h"

#include <iostream>
#include <vector>

#include "../exceptions/InvalidSourceSemanticsException.h"
CallsManager::CallsManager(PKBSPInterface& pkb) : pkb(pkb) {
  calls_extractor = std::make_shared<CallsExtractor>(pkb);
  proc_connector = std::make_shared<CallsProcConnector>(pkb);
}

void CallsManager::InsertProcNode(std::string procName) {
  if (proc_node_map.find(procName) == proc_node_map.end()) {
    std::shared_ptr<CallsGraphProcNode> newNode =
        std::make_shared<CallsGraphProcNode>(procName);
    proc_node_map.insert({procName, newNode});
  } else {
    // throw error: two procs with same name
    throw InvalidSourceSemanticsException(
        "Multiple procedures sharing the same name");
  }
}

// procA: the procedure in which the call stmt is found
// procB: the procedure that the call stmt calls
// i.e. A calls B
void CallsManager::InsertCallsStmt(std::string procA, std::string procB,
                                   std::vector<std::string> actors,
                                   std::shared_ptr<CallNode> callNode) {
  std::shared_ptr<CallsGraphStmtNode> newStmtNode =
      std::make_shared<CallsGraphStmtNode>(actors, callNode);
  std::shared_ptr<CallsGraphProcNode> procCalling = proc_node_map[procA];
  std::shared_ptr<CallsGraphProcNode> procGettingCalled = proc_node_map[procB];

  if (procCalling == nullptr) {
    // invalid proc
    throw InvalidSourceSemanticsException("Proceduce calling does not exist.");
  }
  if (procGettingCalled == nullptr) {
    // throw error: call to non-existant procedure
    throw InvalidSourceSemanticsException(
        "Proceduce getting called does not exist.");
  }
  if (procCalling == procGettingCalled) {
    // throw error: cyclic call detected
    throw InvalidSourceSemanticsException("Proceduce calls itself.");
  }

  procGettingCalled->AddStmtCalledBy(newStmtNode);
  procGettingCalled->AddProcCalledBy(procCalling);
  procCalling->AddProcCalled(procGettingCalled);
}

void CallsManager::ExecuteCallsExtraction() {
  calls_extractor->ExtractCallAbstractions(proc_node_map);
}

void CallsManager::ConnectProcsAndUpdateRelations() {
  proc_connector->ConnectProcsAndUpdateRelations(proc_node_map);
}
