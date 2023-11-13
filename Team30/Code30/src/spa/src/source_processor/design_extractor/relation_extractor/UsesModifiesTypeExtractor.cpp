#include "UsesModifiesTypeExtractor.h"

#include <algorithm>

UsesModifiesTypeExtractor::UsesModifiesTypeExtractor(
    PKBSPInterface& pkb, std::shared_ptr<CallsManager> calls_manager)
    : pkb(pkb), calls_manager(calls_manager), DesignExtractor(pkb) {}

void UsesModifiesTypeExtractor::ExtractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  actors.push_back(node->GetProcedureName());
}

void UsesModifiesTypeExtractor::ExtractFromWhile(
    std::shared_ptr<WhileNode> node) {
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesModifiesTypeExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
  actors.push_back(std::to_string(node->GetStmtIndex()));
}

void UsesModifiesTypeExtractor::PopActor() {
  if (!actors.empty()) {
    actors.pop_back();
  }
}
