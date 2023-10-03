#include "UsesModifiesTypeExtractor.h"

#include <algorithm>
#include <iostream>

UsesModifiesTypeExtractor::UsesModifiesTypeExtractor(PkbApi& pkb, std::shared_ptr<CallsManager> callsManager) : pkb(pkb), callsManager(callsManager), DesignExtractor(pkb) {}

void UsesModifiesTypeExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  actors.push_back(node->getProcedureName());
}

void UsesModifiesTypeExtractor::extractFromCall(
    std::shared_ptr<CallNode> node) {
   callsManager->insertCallsStmt(actors[0], node->getProcName(), actors, node);
}

void UsesModifiesTypeExtractor::extractFromWhile(
    std::shared_ptr<WhileNode> node) {
  actors.push_back(std::to_string(node->getStmtIndex()));
}

void UsesModifiesTypeExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  actors.push_back(std::to_string(node->getStmtIndex()));
}

void UsesModifiesTypeExtractor::popActor() {
  if (!actors.empty()) {
    actors.pop_back();
  }
}

