#include "UsesExtractor.h"

#include <algorithm>
#include <iostream>

UsesExtractor::UsesExtractor(PkbApi& pkb) : pkb(pkb) {}

void UsesExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void UsesExtractor::extractFromProcedure(std::shared_ptr<ProcedureNode> node) {
  usesActors.push_back(node->getProcedureName());
}

void UsesExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  // TODO
}

void UsesExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
  // TODO
}

void UsesExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  pkb.insertRelation(RelationType::USES_S, std::to_string(node->getStmtIndex()),
                     node->getVarName());
  insertVarWithActors(node->getVarName());
}

void UsesExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  // TODO
}

void UsesExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  std::unordered_set<std::string> condVars =
      *node->getCondExpr()->getVariables();
  insertMultipleVars(condVars, std::to_string(node->getStmtIndex()));
  usesActors.push_back(std::to_string(node->getStmtIndex()));
}

void UsesExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  std::unordered_set<std::string> condVars =
      *node->getCondExpr()->getVariables();
  insertMultipleVars(condVars, std::to_string(node->getStmtIndex()));
  usesActors.push_back(std::to_string(node->getStmtIndex()));
}

void UsesExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  std::unordered_set<std::string> rhsVars = *node->getVariables();
  insertMultipleVars(rhsVars, std::to_string(node->getStmtIndex()));
}

//////////////////////////////
//
// PRIVATE HELPER FUNCTIONS
//
//////////////////////////////

void UsesExtractor::popUsesActor() {
  if (!usesActors.empty()) {
    usesActors.pop_back();
  }
}

void UsesExtractor::insertMultipleVars(std::unordered_set<std::string> vars,
                                       std::string stmtIndex) {
  for (std::string var : vars) {
    pkb.insertRelation(RelationType::USES_S, stmtIndex, var);
    insertVarWithActors(var);
  }
}

void UsesExtractor::insertVarWithActors(std::string var) {
  for (std::string usesActor : usesActors) {
    bool isStmtIndex =
        !usesActor.empty() &&
        std::all_of(usesActor.begin(), usesActor.end(), ::isdigit);
    if (isStmtIndex) {
      pkb.insertRelation(RelationType::USES_S, usesActor, var);
    } else {
      pkb.insertRelation(RelationType::USES_P, usesActor, var);
    }
    // std::cout << "(" + usesActor + ", " + var + ")\n";
  }
}
