#include "EntityExtractor.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_set>

EntityExtractor::EntityExtractor(PKBSPInterface& pkb)
    : pkb(pkb), DesignExtractor(pkb) {}

void EntityExtractor::ExtractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  pkb.InsertEntity(EntityType::PROCEDURE, node->GetProcedureName());
}

void EntityExtractor::ExtractFromCall(std::shared_ptr<CallNode> node) {
  pkb.InsertEntity(EntityType::CALL, AttrType::PROC_NAME,
                   std::to_string(node->GetStmtIndex()), node->GetProcName());
}

void EntityExtractor::ExtractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.InsertEntity(EntityType::PRINT, AttrType::VAR_NAME,
                   std::to_string(node->GetStmtIndex()), node->GetVarName());
}

void EntityExtractor::ExtractFromRead(std::shared_ptr<ReadNode> node) {
  pkb.InsertEntity(EntityType::READ, AttrType::VAR_NAME,
                   std::to_string(node->GetStmtIndex()), node->GetVarName());
}

void EntityExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  pkb.InsertEntity(EntityType::WHILE, std::to_string(node->GetStmtIndex()));

  for (std::string var : *node->GetCondExpr()->GetVariables()) {
    pkb.InsertCondVarPattern(EntityType::WHILE,
                             std::to_string(node->GetStmtIndex()), var);
  }
}

void EntityExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
  pkb.InsertEntity(EntityType::IF, std::to_string(node->GetStmtIndex()));

  for (std::string var : *node->GetCondExpr()->GetVariables()) {
    pkb.InsertCondVarPattern(EntityType::IF,
                             std::to_string(node->GetStmtIndex()), var);
  }
}

void EntityExtractor::ExtractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.InsertEntity(EntityType::ASSIGN, std::to_string(node->GetStmtIndex()));

  // Pattern insertion
  pkb.InsertAssignPattern(std::to_string(node->GetStmtIndex()),
                          node->GetVarName(), node->GetRootOfTree());
}
