#include "EntityExtractor.h"

EntityExtractor::EntityExtractor(PkbApi& pkb) : pkb(pkb) {}

void EntityExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node)  {
  // TODO
}

void EntityExtractor::extractFromProcedure(std::shared_ptr<ProcedureNode> node)  {
  // TODO
}

void EntityExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  // TODO
}

void EntityExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
  // TODO
}

void EntityExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  // TODO
}

void EntityExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  // TODO
}

void EntityExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  // TODO
}

void EntityExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  // TODO
}
