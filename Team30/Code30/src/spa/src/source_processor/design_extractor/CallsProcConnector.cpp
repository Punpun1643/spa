#include "CallsProcConnector.h"

#include <algorithm>
#include <cassert>
#include <iostream>

CallsProcConnector::CallsProcConnector(PkbApi& pkb) : pkb(pkb) {}

void CallsProcConnector::connectProcsAndUpdateRelations(
    std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
        procNodeMap) {
  std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
      procNodeMap_copy = procNodeMap;

  while (!procNodeMap_copy.empty()) {
    // traverse procNodeMap to find the proc with the smallest size of
    // procsCalled (should be 0)
    int minCalls = INT_MAX;
    std::shared_ptr<CallsGraphProcNode> nodeWMinCalls;
    std::string procNameWMinCalls;
    for (auto mapNameNode : procNodeMap_copy) {
      int currNumCalls = mapNameNode.second->getNumProcsCalled();
      if (currNumCalls < minCalls) {
        // std::cout << "UPDATEEEEEEEEED\n";
        minCalls = currNumCalls;
        nodeWMinCalls = mapNameNode.second;
        procNameWMinCalls = mapNameNode.first;
      }
    }

    assert(nodeWMinCalls != NULL);

    // checks for cyclic calls
    if (minCalls != 0) {
      // throw error: cyclic call detected
      return;
    }

    // from the pkb, get all the variables that are used / modified
    // by the proc
    std::vector<std::string> usesRelations;
    std::vector<std::string> modifiesRelations;
     usesRelations =
         *pkb.getRelationValues(procNameWMinCalls, EntityType::VARIABLE,
         USES_P);
     modifiesRelations = *pkb.getRelationValues(
         procNameWMinCalls, EntityType::VARIABLE, MODIFIES_P);

    // for each procCalledBy in procsCalls, insert into pkb the relevant uses /
    // modifies relation
    for (std::shared_ptr<CallsGraphStmtNode> stmtCalling :
         nodeWMinCalls->getStmtsCalledBy()) {
      std::shared_ptr<CallNode> node = stmtCalling->getCallNode();
      std::vector<std::string> actors = stmtCalling->getActors();

      // for each stmtCalledBy in stmtsCalledBy, insert into pkb the relevant
      // uses or modifies relation, and with all the actors
       for (std::string usesRelation : usesRelations) {
        pkb.insertRelation(RelationType::USES_S,
                           std::to_string(node->getStmtIndex()),
                           usesRelation);
        for (std::string actor : actors) {
          bool isStmtIndex = !actor.empty() &&
                             std::all_of(actor.begin(), actor.end(),
                             ::isdigit);
          if (isStmtIndex) {
            pkb.insertRelation(RelationType::USES_S, actor, usesRelation);
          } else {
            pkb.insertRelation(RelationType::USES_P, actor, usesRelation);
          }
        }
      }
       for (std::string modifiesRelation : modifiesRelations) {
        pkb.insertRelation(RelationType::USES_S,
                           std::to_string(node->getStmtIndex()),
                           modifiesRelation);
        for (std::string actor : actors) {
          bool isStmtIndex = !actor.empty() &&
                             std::all_of(actor.begin(), actor.end(),
                             ::isdigit);
          if (isStmtIndex) {
            pkb.insertRelation(RelationType::MODIFIES_S, actor,
                               modifiesRelation);
          } else {
            pkb.insertRelation(RelationType::MODIFIES_P, actor,
                               modifiesRelation);
          }
        }
      }
    }

    // remove this procNode from the procNodeMap
    for (std::shared_ptr<CallsGraphProcNode> procCalling :
         nodeWMinCalls->getProcsCalledBy()) {
      procCalling->removeProcCalled(nodeWMinCalls);
    }

    procNodeMap_copy.erase(procNameWMinCalls);
  }
}