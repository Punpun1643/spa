#include "ModifiesExtractor.h"

#include <iostream>
#include <vector>

ModifiesExtractor::ModifiesExtractor(PkbApi& pkb) : pkb(pkb) {}

void ModifiesExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  modifyActors.push_back(node->getProcedureName());
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
  /*pkb.insertRelation(RelationType::MODIFIES,
                     std::to_string(node->getStmtIndex()), node->getVarName());
  for (std::string modifyActor : modifyActors) {
    pkb.insertRelation(RelationType::MODIFIES, modifyActor, node->getVarName());
  }*/
}

void ModifiesExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  modifyActors.push_back(std::to_string(node->getStmtIndex()));
}

void ModifiesExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  modifyActors.push_back(std::to_string(node->getStmtIndex()));
}

void ModifiesExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  // TODO
}
