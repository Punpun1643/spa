#include "UsesModifiesTypeExtractor.h"

#include <algorithm>
#include <iostream>

UsesModifiesTypeExtractor::UsesModifiesTypeExtractor(PkbApi& pkb) : pkb(pkb), DesignExtractor(pkb) {}

void UsesModifiesTypeExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  actors.push_back(node->getProcedureName());
}

void UsesModifiesTypeExtractor::extractFromCall(
    std::shared_ptr<CallNode> node) {
  CallStmtCacheObject newCallStmt = CallStmtCacheObject(actors, node);
  callStmtCache.push_back(std::make_shared<CallStmtCacheObject>(newCallStmt));
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

std::vector<std::shared_ptr<CallStmtCacheObject>>
UsesModifiesTypeExtractor::getCallStmtCache() {
  return callStmtCache;
}

