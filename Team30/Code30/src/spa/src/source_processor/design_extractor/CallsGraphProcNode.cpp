#include <iterator>

#include "CallsGraphProcNode.h"

CallsGraphProcNode::CallsGraphProcNode(std::string proc) : proc(proc) {}

void CallsGraphProcNode::addStmtCalledBy(
    std::shared_ptr<CallsGraphStmtNode> stmtCalledBy) {
  stmtsCalledBy.insert(stmtCalledBy);
}

void CallsGraphProcNode::addProcCalled(
    std::shared_ptr<CallsGraphProcNode> procCalled) {
  procsCalled.insert(procCalled);
}

void CallsGraphProcNode::addProcCalledBy(
    std::shared_ptr<CallsGraphProcNode> procCalledBy) {
  procsCalledBy.insert(procCalledBy);
}

