#include "ModifiesExtractor.h"

#include <algorithm>
#include <iostream>
#include <vector>

ModifiesExtractor::ModifiesExtractor(PKBSPInterface& pkb,
                                     std::shared_ptr<CallsManager> callsManager)
    : pkb(pkb),
      callsManager(callsManager),
      UsesModifiesTypeExtractor(pkb, callsManager) {}

void ModifiesExtractor::ExtractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ModifiesExtractor::ExtractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.insertRelation(RelationType::MODIFIES_S,
                     std::to_string(node->getStmtIndex()), node->getVarName());
  InsertVarWithActors(node->getVarName());
}

void ModifiesExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertRelation(RelationType::MODIFIES_S,
                     std::to_string(node->getStmtIndex()), node->getVarName());
  InsertVarWithActors(node->getVarName());
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void ModifiesExtractor::InsertVarWithActors(std::string var) {
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
