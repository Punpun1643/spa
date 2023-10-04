#pragma once

#include <source_processor/node/stmt_node/CallNode.h>

#include <memory>
#include <unordered_set>

#include "CallsGraphStmtNode.h"

class CallsGraphProcNode {
 public:
  CallsGraphProcNode(std::string proc);

  void addStmtCalledBy(std::shared_ptr<CallsGraphStmtNode> stmtCalledBy);

  void addProcCalled(std::shared_ptr<CallsGraphProcNode> procCalled);

  void addProcCalledBy(std::shared_ptr<CallsGraphProcNode> procCalledBy);

  std::unordered_set<std::shared_ptr<CallsGraphProcNode>> getProcsCalled();

  std::unordered_set<std::shared_ptr<CallsGraphStmtNode>> getStmtsCalledBy();

  ~CallsGraphProcNode() = default;

 private:
  std::string proc;

  // Call stmts that call this procedure
  std::unordered_set<std::shared_ptr<CallsGraphStmtNode>> stmtsCalledBy;

  // Procedures that this procedure calls
  std::unordered_set<std::shared_ptr<CallsGraphProcNode>> procsCalled;

  // Procedures that call this procedure 
  std::unordered_set<std::shared_ptr<CallsGraphProcNode>> procsCalledBy;
};