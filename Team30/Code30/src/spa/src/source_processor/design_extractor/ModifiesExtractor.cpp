#include "ModifiesExtractor.h"

#include <algorithm>
#include <iostream>
#include <vector>

ModifiesExtractor::ModifiesExtractor(PKBSPInterface& pkb,
                                     std::shared_ptr<CallsManager> callsManager)
    : pkb(pkb),
      callsManager(callsManager),
      UsesModifiesTypeExtractor(pkb, callsManager) {}

void ModifiesExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.insertRelation(RelationType::MODIFIES_S,
                     std::to_string(node->GetStmtIndex()), node->GetVarName());
  insertVarWithActors(node->GetVarName());
}

void ModifiesExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertRelation(RelationType::MODIFIES_S,
                     std::to_string(node->GetStmtIndex()), node->GetVarName());
  insertVarWithActors(node->GetVarName());
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void ModifiesExtractor::insertVarWithActors(std::string var) {
  for (std::string modifiesActor : actors) {
    // insertIntoPkb(modifiesActor, var);
    bool isStmtIndex =
        !modifiesActor.empty() &&
        std::all_of(modifiesActor.begin(), modifiesActor.end(), ::isdigit);
    if (isStmtIndex) {
      pkb.insertRelation(RelationType::MODIFIES_S, modifiesActor, var);
    } else {
      pkb.insertRelation(RelationType::MODIFIES_P, modifiesActor, var);
    }
  }
}
