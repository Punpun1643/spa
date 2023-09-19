#include "ParentExtractor.h"

#include <iostream>
#include <vector>

ParentExtractor::ParentExtractor(PkbApi& pkb) : pkb(pkb) {}

void ParentExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void ParentExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
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
  std::vector<std::shared_ptr<StmtNode>> children =
      node->getStmtLst()->getChildren();
  for (int i = 0; i < children.size(); i++) {
    pkb.insertRelation(RelationType::PARENT,
                       std::to_string(node->getStmtIndex()),
                       std::to_string(children[i]->getStmtIndex()));
  }
}

void ParentExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  int parentIndex = node->getStmtIndex();
  std::vector<std::shared_ptr<StmtNode>> children =
      node->getThenStmtLst()->getChildren();
  std::vector<std::shared_ptr<StmtNode>> elseChildren =
      node->getElseStmtLst()->getChildren();
  children.insert(std::end(children), std::begin(elseChildren),
                  std::end(elseChildren));
  for (int i = 0; i < children.size(); i++) {
    pkb.insertRelation(RelationType::PARENT,
                       std::to_string(node->getStmtIndex()),
                       std::to_string(children[i]->getStmtIndex()));
  }
}

void ParentExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {
  // TODO
}
