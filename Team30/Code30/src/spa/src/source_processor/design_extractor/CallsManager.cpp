#include "CallsManager.h"

#include <source_processor/exceptions/InvalidSourceSemanticsException.h>

#include <iostream>
#include <vector>
CallsManager::CallsManager(PKBSPInterface& pkb) : pkb(pkb) {
  callsExtractor = std::make_shared<CallsExtractor>(pkb);
  procConnector = std::make_shared<CallsProcConnector>(pkb);
}

void CallsManager::insertProcNode(std::string procName) {
  if (procNodeMap.find(procName) == procNodeMap.end()) {
    std::shared_ptr<CallsGraphProcNode> newNode =
        std::make_shared<CallsGraphProcNode>(procName);
    procNodeMap.insert({procName, newNode});
  } else {
    // throw error: two procs with same name
    throw InvalidSourceSemanticsException(
        "Multiple procedures sharing the same name");
  }
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
