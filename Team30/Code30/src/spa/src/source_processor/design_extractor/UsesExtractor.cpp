#include "UsesExtractor.h"
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
  pkb.insertRelation(RelationType::USES, std::to_string(node->getStmtIndex()),
                     node->getVarName());
  for (std::string usesActor : usesActors) {
    pkb.insertRelation(RelationType::USES, usesActor, node->getVarName());
  }
}

void UsesExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  // TODO
}

void UsesExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
  std::unordered_set<std::string> condVars = node->getCondExpr()-> getVariables();
  
  // Can potentially be extracted for while and if
  for (std::string condVar : condVars) {
    pkb.insertRelation(RelationType::USES, std::to_string(node->getStmtIndex()), condVar);
    for (std::string usesActor : usesActors) {
      pkb.insertRelation(RelationType::USES, usesActor, condVar);
    }
  }
  usesActors.push_back(std::to_string(node->getStmtIndex()));
}

void UsesExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  std::unordered_set<std::string> condVars =
      node->getCondExprNode()->getVariables();

  // Can potentially be extracted for while and if
  for (std::string condVar : condVars) {
    pkb.insertRelation(RelationType::USES, std::to_string(node->getStmtIndex()),
                       condVar);
    for (std::string usesActor : usesActors) {
      pkb.insertRelation(RelationType::USES, usesActor, condVar);
    }
  }
  usesActors.push_back(std::to_string(node->getStmtIndex()));
}

// void UsesExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {}

void UsesExtractor::popUsesActors() {
  if (!usesActors.empty()) {
    usesActors.pop_back();
  }
}
