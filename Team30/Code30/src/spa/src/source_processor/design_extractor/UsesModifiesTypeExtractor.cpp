#include "UsesModifiesTypeExtractor.h"

#include <algorithm>
#include <iostream>

UsesModifiesTypeExtractor::UsesModifiesTypeExtractor(
    PKBSPInterface& pkb, std::shared_ptr<CallsManager> callsManager)
    : pkb(pkb), callsManager(callsManager), DesignExtractor(pkb) {}

void UsesModifiesTypeExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  actors.push_back(node->GetProcedureName());
}

void UsesModifiesTypeExtractor::extractFromWhile(
    std::shared_ptr<WhileNode> node) {
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesModifiesTypeExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesModifiesTypeExtractor::popActor() {
  if (!actors.empty()) {
    actors.pop_back();
  }
}
