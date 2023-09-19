#include "EntityExtractor.h"

EntityExtractor::EntityExtractor(PkbApi& pkb) : pkb(pkb) {}

void EntityExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void EntityExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
   pkb.insertEntity(EntityType::PROCEDURE, node->getProcedureName());
}

void EntityExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  // TODO
}

void EntityExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
   pkb.insertEntity(EntityType::CALL, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
   pkb.insertEntity(EntityType::PRINT, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
   pkb.insertEntity(EntityType::READ, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
   pkb.insertEntity(EntityType::WHILE, std::to_string(node->getStmtIndex()));
}

void EntityExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
   pkb.insertEntity(EntityType::IF, std::to_string(node->getStmtIndex()));
}

// void EntityExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {}