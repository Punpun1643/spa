#include "CallsManager.h"

#include <vector>

#include "../exceptions/InvalidSourceSemanticsException.h"
CallsManager::CallsManager(PKBSPInterface& pkb) : pkb(pkb) {
  calls_extractor = std::make_shared<CallsExtractor>(pkb);
  proc_connector = std::make_shared<CallsProcConnector>(pkb);
}

void CallsManager::InsertProcNode(std::string proc_name) {
  if (proc_node_map.find(proc_name) == proc_node_map.end()) {
    std::shared_ptr<CallsGraphProcNode> new_node =
        std::make_shared<CallsGraphProcNode>(proc_name);
    proc_node_map.insert({proc_name, new_node});
  } else {
    // throw error: two procs with same name
    throw InvalidSourceSemanticsException(
        "Multiple procedures sharing the same name");
  }
}

// procA: the procedure in which the call stmt is found
// procB: the procedure that the call stmt calls
// i.e. A calls B
void CallsManager::InsertCallsStmt(std::string proc_a, std::string proc_b,
                                   std::vector<std::string> actors,
                                   std::shared_ptr<CallNode> call_node) {
  std::shared_ptr<CallsGraphStmtNode> new_stmt_node =
      std::make_shared<CallsGraphStmtNode>(actors, call_node);
  std::shared_ptr<CallsGraphProcNode> proc_calling = proc_node_map[proc_a];
  std::shared_ptr<CallsGraphProcNode> proc_getting_called =
      proc_node_map[proc_b];

  if (proc_calling == nullptr) {
    // invalid proc
    throw InvalidSourceSemanticsException("Proceduce calling does not exist.");
  }
  if (proc_getting_called == nullptr) {
    // throw error: call to non-existant procedure
    throw InvalidSourceSemanticsException(
        "Proceduce getting called does not exist.");
  }
  if (proc_calling == proc_getting_called) {
    // throw error: cyclic call detected
    throw InvalidSourceSemanticsException("Proceduce calls itself.");
  }

  proc_getting_called->AddStmtCalledBy(new_stmt_node);
  proc_getting_called->AddProcCalledBy(proc_calling);
  proc_calling->AddProcCalled(proc_getting_called);
}

void CallsManager::ExecuteCallsExtraction() {
  calls_extractor->ExtractCallAbstractions(proc_node_map);
}

void CallsManager::ConnectProcsAndUpdateRelations() {
  proc_connector->ConnectProcsAndUpdateRelations(proc_node_map);
}
