#include "ModifiesExtractor.h"

#include <algorithm>
#include <iostream>
#include <vector>

ModifiesExtractor::ModifiesExtractor(PKBSPInterface& pkb,
                                     std::shared_ptr<CallsManager> calls_manager)
    : pkb(pkb),
      calls_manager(calls_manager),
      UsesModifiesTypeExtractor(pkb, calls_manager) {}

void ModifiesExtractor::ExtractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.insertRelation(RelationType::MODIFIES_S,
                     std::to_string(node->GetStmtIndex()), node->GetVarName());
  InsertVarWithActors(node->GetVarName());
}

void ModifiesExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertRelation(RelationType::MODIFIES_S,
                     std::to_string(node->GetStmtIndex()), node->GetVarName());
  InsertVarWithActors(node->GetVarName());
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void ModifiesExtractor::InsertVarWithActors(std::string var) {
  for (std::string modifies_actor : actors) {
    // insertIntoPkb(modifiesActor, var);
    bool is_stmt_index =
        !modifies_actor.empty() &&
        std::all_of(modifies_actor.begin(), modifies_actor.end(), ::isdigit);
    if (is_stmt_index) {
      pkb.insertRelation(RelationType::MODIFIES_S, modifies_actor, var);
    } else {
      pkb.insertRelation(RelationType::MODIFIES_P, modifies_actor, var);
    }
  }
}
