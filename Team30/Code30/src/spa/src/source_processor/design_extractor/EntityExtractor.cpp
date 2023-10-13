#include "EntityExtractor.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>

EntityExtractor::EntityExtractor(PKBSPInterface& pkb) : pkb(pkb) {}

void EntityExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void EntityExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  pkb.insertEntity(EntityType::PROCEDURE, node->GetProcedureName());
}

void EntityExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  // TODO
}

void EntityExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
  pkb.insertEntity(EntityType::CALL, std::to_string(node->GetStmtIndex()));
}

void EntityExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.insertEntity(EntityType::PRINT, std::to_string(node->GetStmtIndex()));
}

void EntityExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.insertEntity(EntityType::READ, std::to_string(node->GetStmtIndex()));
}

void EntityExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  pkb.insertEntity(EntityType::WHILE, std::to_string(node->GetStmtIndex()));
}

void EntityExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  pkb.insertEntity(EntityType::IF, std::to_string(node->GetStmtIndex()));
}

void EntityExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertEntity(EntityType::ASSIGN, std::to_string(node->GetStmtIndex()));

  // Pattern insertion
  pkb.insertPattern(PatternType::ASSIGN, std::to_string(node->GetStmtIndex()), node->GetVarName(),
                    node->GetRootOfTree());
}
