#pragma once

#include <source_processor/node/stmt_node/CallNode.h>

#include <memory>
#include <unordered_set>

#include "CallsGraphStmtNode.h"

class CallsGraphProcNode {
 public:
  CallsGraphProcNode(std::string proc);

  void AddStmtCalledBy(std::shared_ptr<CallsGraphStmtNode> stmtCalledBy);

  void AddProcCalled(std::shared_ptr<CallsGraphProcNode> procCalled);

  void AddProcCalledBy(std::shared_ptr<CallsGraphProcNode> procCalledBy);

  std::unordered_set<std::shared_ptr<CallsGraphProcNode>> GetProcsCalled();

  std::unordered_set<std::shared_ptr<CallsGraphStmtNode>> GetStmtsCalledBy();

  std::unordered_set<std::shared_ptr<CallsGraphProcNode>> GetProcsCalledBy();

  std::string GetProcName();

  int GetNumProcsCalled();

  void RemoveProcCalled(std::shared_ptr<CallsGraphProcNode> proc); 

  ~CallsGraphProcNode() = default;

 private:
  std::string proc;

  int num_procs_called;

  // Call stmts that call this procedure
  std::unordered_set<std::shared_ptr<CallsGraphStmtNode>> stmts_called_by;

  // Procedures that this procedure calls
  std::unordered_set<std::shared_ptr<CallsGraphProcNode>> procs_called;

  // Procedures that call this procedure 
  std::unordered_set<std::shared_ptr<CallsGraphProcNode>> procs_called_by;
};