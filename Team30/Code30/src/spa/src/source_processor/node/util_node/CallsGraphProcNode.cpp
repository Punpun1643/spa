#include "CallsGraphProcNode.h"

#include <cassert>
#include <iterator>

CallsGraphProcNode::CallsGraphProcNode(std::string proc)
    : proc(proc), num_procs_called(0) {}

void CallsGraphProcNode::AddStmtCalledBy(
    std::shared_ptr<CallsGraphStmtNode> stmt_called_by) {
  if (stmts_called_by.find(stmt_called_by) == stmts_called_by.end()) {
    stmts_called_by.insert(stmt_called_by);
  }
}

void CallsGraphProcNode::AddProcCalled(
    std::shared_ptr<CallsGraphProcNode> proc_called) {
  if (procs_called.find(proc_called) == procs_called.end()) {
    procs_called.insert(proc_called);
    num_procs_called++;
  }
}

void CallsGraphProcNode::AddProcCalledBy(
    std::shared_ptr<CallsGraphProcNode> proc_called_by) {
  procs_called_by.insert(proc_called_by);
}

std::unordered_set<std::shared_ptr<CallsGraphProcNode>>
CallsGraphProcNode::GetProcsCalled() {
  return procs_called;
}

std::unordered_set<std::shared_ptr<CallsGraphStmtNode>>
CallsGraphProcNode::GetStmtsCalledBy() {
  return stmts_called_by;
}

std::unordered_set<std::shared_ptr<CallsGraphProcNode>>
CallsGraphProcNode::GetProcsCalledBy() {
  return procs_called_by;
}

int CallsGraphProcNode::GetNumProcsCalled() { return num_procs_called; }

void CallsGraphProcNode::RemoveProcCalled(
    std::shared_ptr<CallsGraphProcNode> proc) {
  procs_called.erase(proc);
  num_procs_called--;
}

std::string CallsGraphProcNode::GetProcName() { return proc; }
