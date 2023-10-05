#include "UsesExtractor.h"

#include <algorithm>
#include <iostream>

UsesExtractor::UsesExtractor(PkbApi& pkb,
                             std::shared_ptr<CallsManager> callsManager)
    : pkb(pkb),
      callsManager(callsManager),
      UsesModifiesTypeExtractor(pkb, callsManager) {}

void UsesExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
  callsManager->insertCallsStmt(actors[0], node->getProcName(), actors, node);
}

void UsesExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.insertRelation(RelationType::USES_S, std::to_string(node->getStmtIndex()),
                     node->getVarName());
  insertVarWithActors(node->getVarName());
}

void UsesExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  std::unordered_set<std::string> condVars =
      *node->getCondExpr()->getVariables();
  insertMultipleVars(condVars, std::to_string(node->getStmtIndex()));
  actors.push_back(std::to_string(node->getStmtIndex()));
}

void UsesExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  std::unordered_set<std::string> condVars =
      *node->getCondExpr()->getVariables();
  insertMultipleVars(condVars, std::to_string(node->getStmtIndex()));
  actors.push_back(std::to_string(node->getStmtIndex()));
}

void UsesExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  std::unordered_set<std::string> rhsVars = *node->getVariables();
  insertMultipleVars(rhsVars, std::to_string(node->getStmtIndex()));
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void UsesExtractor::insertMultipleVars(std::unordered_set<std::string> vars,
                                       std::string stmtIndex) {
  for (std::string var : vars) {
    pkb.insertRelation(RelationType::USES_S, stmtIndex, var);
    insertVarWithActors(var);
  }
}

void UsesExtractor::insertVarWithActors(std::string var) {
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
