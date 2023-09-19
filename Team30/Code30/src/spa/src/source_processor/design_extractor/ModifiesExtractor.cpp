#include "ModifiesExtractor.h"

#include <iostream>
#include <vector>

ModifiesExtractor::ModifiesExtractor(PkbApi& pkb) : pkb(pkb) {}

void ModifiesExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ModifiesExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  modifiesActors.push_back(node->getProcedureName());
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
  insertIntoPkb(std::to_string(node->getStmtIndex()), node->getVarName());
  insertVarWithActors(node->getVarName());
}

void ModifiesExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  modifiesActors.push_back(std::to_string(node->getStmtIndex()));
}

void ModifiesExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  modifiesActors.push_back(std::to_string(node->getStmtIndex()));
}

void ModifiesExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  pkb.insertRelation(RelationType::MODIFIES,
                     std::to_string(node->getStmtIndex()), node->getVarName());
  insertVarWithActors(node->getVarName());
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void ModifiesExtractor::insertIntoPkb(std::string actor, std::string var) {
  pkb.insertRelation(RelationType::MODIFIES, actor, var);
  // std::cout << "MODS (" + actor + ", " + var + ")\n";
}

void ModifiesExtractor::popModifiesActor() {
  if (!modifiesActors.empty()) {
    modifiesActors.pop_back();
  }
}

void ModifiesExtractor::insertVarWithActors(std::string var) {
  for (std::string modifiesActor : modifiesActors) {
    insertIntoPkb(modifiesActor, var);
  }
}
