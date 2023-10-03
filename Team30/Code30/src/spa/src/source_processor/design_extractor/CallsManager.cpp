#include "CallsManager.h"

#include <vector>
CallsManager::CallsManager() {}

void CallsManager::insertProcNode(std::string procName) {
  std::shared_ptr<CallsGraphProcNode> newNode =
      std::make_shared<CallsGraphProcNode>(procName);
  procNodeMap.insert({procName, newNode});
}

void CallsManager::insertCallsStmt(std::string procCalled,
                                   std::string procCalling,
                                   std::vector<std::string> actors) {
  std::shared_ptr<CallsGraphStmtNode> newStmtNode =
      std::make_shared<CallsGraphStmtNode>(actors);
  std::shared_ptr<CallsGraphProcNode> procCalledNode = procNodeMap.at(procCalled);
  std::shared_ptr<CallsGraphProcNode> procCallingNode =
      procNodeMap.at(procCalling);

  procCalledNode->addStmtCalledBy(newStmtNode);
  procCalledNode->addProcCalledBy(procCalledNode);
  procCallingNode->addProcCalled(procCallingNode);
}

void CallsManager::executeCallsGraphTraversal() {}
