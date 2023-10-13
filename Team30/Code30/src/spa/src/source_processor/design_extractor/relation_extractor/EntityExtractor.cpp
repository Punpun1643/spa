#include "EntityExtractor.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>

EntityExtractor::EntityExtractor(PKBSPInterface& pkb)
    : pkb(pkb), DesignExtractor(pkb) {}

void EntityExtractor::ExtractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  pkb.insertEntity(EntityType::PROCEDURE, node->GetProcedureName());
}

void EntityExtractor::ExtractFromCall(std::shared_ptr<CallNode> node) {
  pkb.insertEntity(EntityType::CALL, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::ExtractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.insertEntity(EntityType::PRINT, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::ExtractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.insertEntity(EntityType::READ, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  pkb.insertEntity(EntityType::WHILE, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
  pkb.insertEntity(EntityType::IF, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertEntity(EntityType::ASSIGN, std::to_string(node->getStmtIndex()));

  // Pattern insertion
  pkb.insertPattern(PatternType::ASSIGN, std::to_string(node->GetStmtIndex()), node->GetVarName(),
                    node->GetRootOfTree());
}
