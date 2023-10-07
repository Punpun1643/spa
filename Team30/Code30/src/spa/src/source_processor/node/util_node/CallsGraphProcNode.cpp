#include "CallsGraphProcNode.h"

#include <cassert>
#include <iterator>

CallsGraphProcNode::CallsGraphProcNode(std::string proc)
    : proc(proc), numProcsCalled(0) {}

void CallsGraphProcNode::addStmtCalledBy(
    std::shared_ptr<CallsGraphStmtNode> stmtCalledBy) {
  if (stmtsCalledBy.find(stmtCalledBy) == stmtsCalledBy.end()) {
    stmtsCalledBy.insert(stmtCalledBy);
  }
}

void CallsGraphProcNode::addProcCalled(
    std::shared_ptr<CallsGraphProcNode> procCalled) {
  if (procsCalled.find(procCalled) == procsCalled.end()) {
    procsCalled.insert(procCalled);
    numProcsCalled++;
  }
}

void CallsGraphProcNode::addProcCalledBy(
    std::shared_ptr<CallsGraphProcNode> procCalledBy) {
  procsCalledBy.insert(procCalledBy);
}

std::unordered_set<std::shared_ptr<CallsGraphProcNode>>
CallsGraphProcNode::getProcsCalled() {
  return procsCalled;
}

std::unordered_set<std::shared_ptr<CallsGraphStmtNode>>
CallsGraphProcNode::getStmtsCalledBy() {
  return stmtsCalledBy;
}

std::unordered_set<std::shared_ptr<CallsGraphProcNode>>
CallsGraphProcNode::getProcsCalledBy() {
  return procsCalledBy;
}

int CallsGraphProcNode::getNumProcsCalled() { return numProcsCalled; }

void CallsGraphProcNode::removeProcCalled(
    std::shared_ptr<CallsGraphProcNode> proc) {
  procsCalled.erase(proc);
  numProcsCalled--;
}

std::string CallsGraphProcNode::getProcName() { return proc; }
