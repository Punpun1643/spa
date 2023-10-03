#include "CallsManager.h"

#include <vector>
#include <iostream>
CallsManager::CallsManager() {}

void CallsManager::insertProcNode(std::string procName) {
  std::shared_ptr<CallsGraphProcNode> newNode =
      std::make_shared<CallsGraphProcNode>(procName);
   procNodeMap.insert({procName, newNode});
}

void CallsManager::insertCallsStmt(std::string procCalled,
                                   std::string procCalling,
                                   std::vector<std::string> actors,
                                   std::shared_ptr<CallNode> callNode) {
  std::shared_ptr<CallsGraphStmtNode> newStmtNode =
      std::make_shared<CallsGraphStmtNode>(actors, callNode);
  std::shared_ptr<CallsGraphProcNode> procCalledNode =
      procNodeMap[procCalled];
  std::shared_ptr<CallsGraphProcNode> procCallingNode =
      procNodeMap[procCalling];
  if (procCalledNode != NULL && procCallingNode != NULL) {
    procCalledNode->addStmtCalledBy(newStmtNode);
    procCalledNode->addProcCalledBy(procCalledNode);
    procCallingNode->addProcCalled(procCallingNode);
  } else {
      // Throw error -> procedure calls itself (cyclic) or 
      //                calls a procedure that does not exist
  }
}

void CallsManager::executeCallsGraphTraversal() {
  // traverse procNodeMap to find the proc with the smallest size of procsCalled
  // (should be 0)

  // from the pkb, get all the variables that are used / modified
  // by the proc

  // for each procCalledBy in procsCalls, insert into pkb the relevant uses /
  // modifies relation

  // for each stmtCalledBy in stmtsCalledBy, insert into pkb the relevant uses /
  // modifies relation, and with all the actors

  // remove this procNode from the procNodeMap

  // loop until no more procs in procNodeMap
}
