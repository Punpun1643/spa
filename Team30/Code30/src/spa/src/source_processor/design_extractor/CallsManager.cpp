#include "CallsManager.h"

#include <iostream>
#include <vector>
CallsManager::CallsManager(PkbApi& pkb) : pkb(pkb) {
  callsExtractor = std::make_shared<CallsExtractor>(pkb);
  procConnector = std::make_shared<CallsProcConnector>(pkb);
}

void CallsManager::insertProcNode(std::string procName) {
  std::shared_ptr<CallsGraphProcNode> newNode =
      std::make_shared<CallsGraphProcNode>(procName);
  procNodeMap.insert({procName, newNode});
  //std::cout << "INSERTED " + procName + "\n";
}

// procA: the procedure in which the call stmt is found
// procB: the procedure that the call stmt calls
// i.e. A calls B
void CallsManager::insertCallsStmt(std::string procA, std::string procB,
                                   std::vector<std::string> actors,
                                   std::shared_ptr<CallNode> callNode) {
  std::shared_ptr<CallsGraphStmtNode> newStmtNode =
      std::make_shared<CallsGraphStmtNode>(actors, callNode);

  std::shared_ptr<CallsGraphProcNode> procCalling = procNodeMap[procA];
  std::shared_ptr<CallsGraphProcNode> procGettingCalled = procNodeMap[procB];

  if (procCalling == NULL) {
    // invalid proc
    std::cout << "ERROR: procCalling == NULL\n";
    return;
  }
  if (procGettingCalled == NULL) {
    // throw error: call to non-existant procedure
    std::cout << "ERROR: procGettingCalled == NULL\n";
    return;
  }
  if (procCalling == procGettingCalled) {
    // throw error: cyclic call detected
    std::cout << "ERROR: same name\n";
    return;
  }

  procGettingCalled->addStmtCalledBy(newStmtNode);
  procGettingCalled->addProcCalledBy(procCalling);
  procCalling->addProcCalled(procGettingCalled);
}

void CallsManager::executeCallsExtraction() {
  callsExtractor->extractCallAbstractions(procNodeMap);
}

void CallsManager::connectProcsAndUpdateRelations() {
  procConnector->connectProcsAndUpdateRelations(procNodeMap);
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
