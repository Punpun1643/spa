#include "CallsProcConnector.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "../exceptions/InvalidSourceSemanticsException.h"

CallsProcConnector::CallsProcConnector(PKBSPInterface& pkb) : pkb(pkb) {}

void CallsProcConnector::ConnectProcsAndUpdateRelations(
    std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
        procNodeMap) {
  std::unordered_map<std::string, std::shared_ptr<CallsGraphProcNode>>
      procNodeMap_copy = procNodeMap;

  while (!procNodeMap_copy.empty()) {
    // for (auto m : procNodeMap_copy) {
    // std::cout << m.first + "\n";
    // }

    // traverse procNodeMap to find the proc with the smallest size of
    // procsCalled (should be 0)
    int minCalls = INT_MAX;
    std::shared_ptr<CallsGraphProcNode> nodeWMinCalls;
    std::string procNameWMinCalls;
    for (auto mapNameNode : procNodeMap_copy) {
      int currNumCalls = mapNameNode.second->GetNumProcsCalled();
      if (currNumCalls < minCalls) {
        minCalls = currNumCalls;
        nodeWMinCalls = mapNameNode.second;
        procNameWMinCalls = mapNameNode.first;
      }
    }

    assert(nodeWMinCalls != NULL);

    // checks for cyclic calls
    if (minCalls != 0) {
      throw InvalidSourceSemanticsException("Cyclic call detected.");
    }

    // from the pkb, get all the variables that are used / modified
    // by the proc

    std::unordered_set<std::string> usesRelations =
        pkb.getProcedureUses(procNameWMinCalls);
    std::unordered_set<std::string> modifiesRelations =
        pkb.getProcedureModifies(procNameWMinCalls);

    // for each procCalledBy in procsCalls, insert into pkb the relevant uses /
    // modifies relation
    for (std::shared_ptr<CallsGraphStmtNode> stmtCalling :
         nodeWMinCalls->GetStmtsCalledBy()) {
      std::shared_ptr<CallNode> node = stmtCalling->GetCallNode();
      std::vector<std::string> actors = stmtCalling->GetActors();

      // for each stmtCalledBy in stmtsCalledBy, insert into pkb the relevant
      // uses or modifies relation, and with all the actors
      for (std::string usesRelation : usesRelations) {
        pkb.insertRelation(RelationType::USES_S,
                           std::to_string(node->GetStmtIndex()), usesRelation);
        for (std::string actor : actors) {
          bool isStmtIndex = !actor.empty() &&
                             std::all_of(actor.begin(), actor.end(), ::isdigit);
          if (isStmtIndex) {
            pkb.insertRelation(RelationType::USES_S, actor, usesRelation);
          } else {
            pkb.insertRelation(RelationType::USES_P, actor, usesRelation);
          }
        }
      }
      for (std::string modifiesRelation : modifiesRelations) {
        pkb.insertRelation(RelationType::MODIFIES_S,
                           std::to_string(node->GetStmtIndex()),
                           modifiesRelation);
        for (std::string actor : actors) {
          bool isStmtIndex = !actor.empty() &&
                             std::all_of(actor.begin(), actor.end(), ::isdigit);
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
         nodeWMinCalls->GetProcsCalledBy()) {
      procCalling->RemoveProcCalled(nodeWMinCalls);
    }

    procNodeMap_copy.erase(procNameWMinCalls);
  }
}