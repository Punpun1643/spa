#include <iostream>
#include <vector>

#include "ModifiesExtractor.h"

ModifiesExtractor::ModifiesExtractor(PkbApi& pkb) : pkb(pkb) {}

void ModifiesExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromProcedure(std::shared_ptr<ProcedureNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  // TODO
}

