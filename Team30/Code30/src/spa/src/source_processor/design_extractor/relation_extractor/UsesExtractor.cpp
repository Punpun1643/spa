#include "UsesExtractor.h"

#include <algorithm>
#include <unordered_set>
#include <string>

UsesExtractor::UsesExtractor(PKBSPInterface& pkb,
                             std::shared_ptr<CallsManager> callsManager)
    : pkb(pkb),
      callsManager(callsManager),
      UsesModifiesTypeExtractor(pkb, callsManager) {}

void UsesExtractor::ExtractFromCall(std::shared_ptr<CallNode> node) {
  callsManager->InsertCallsStmt(actors[0], node->GetProcName(), actors, node);
}

void UsesExtractor::ExtractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.insertRelation(RelationType::USES_S, std::to_string(node->GetStmtIndex()),
                     node->GetVarName());
  InsertVarWithActors(node->GetVarName());
}

void UsesExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  std::unordered_set<std::string> condVars =
      *node->GetCondExpr()->GetVariables();
  InsertMultipleVars(condVars, std::to_string(node->GetStmtIndex()));
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
  std::unordered_set<std::string> condVars =
      *node->GetCondExpr()->GetVariables();
  InsertMultipleVars(condVars, std::to_string(node->GetStmtIndex()));
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {
  std::unordered_set<std::string> rhsVars = *node->GetVariables();
  InsertMultipleVars(rhsVars, std::to_string(node->GetStmtIndex()));
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void UsesExtractor::InsertMultipleVars(std::unordered_set<std::string> vars,
                                       std::string stmtIndex) {
  for (std::string var : vars) {
    pkb.insertRelation(RelationType::USES_S, stmtIndex, var);
    InsertVarWithActors(var);
  }
}

void UsesExtractor::InsertVarWithActors(std::string var) {
  for (std::string usesActor : actors) {
    bool isStmtIndex =
        !usesActor.empty() &&
        std::all_of(usesActor.begin(), usesActor.end(), ::isdigit);
    if (isStmtIndex) {
      pkb.insertRelation(RelationType::USES_S, usesActor, var);
    } else {
      pkb.insertRelation(RelationType::USES_P, usesActor, var);
    }
    // std::cout << "(" + usesActor + ", " + var + ")\n";
  }
}
