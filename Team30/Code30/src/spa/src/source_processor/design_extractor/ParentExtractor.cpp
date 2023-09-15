#include <iostream>
#include <vector>

#include "ParentExtractor.h"

ParentExtractor::ParentExtractor(PkbApi& pkb) : pkb(pkb) {}

void ParentExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ParentExtractor::extractFromProcedure(std::shared_ptr<ProcedureNode> node) {
  // TODO
}

void ParentExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  // TODO
}

void ParentExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
  // TODO
}

void ParentExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  // TODO
}

void ParentExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  // TODO
}

void ParentExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  // TODO
}

void ParentExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  // TODO
}
