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

//// If WhileNode and IfNode inherit from the same type, can extract out the
//// functions into a private call.
// void ParentExtractor::extractFromWhile(std::shared_ptr<WhileNode> node) {
//   int parentIndex = node->getStmtIndex();
//   std::vector<std::shared_ptr<StmtNode>> children = node->getStmtLstNode()->getChildren();
//   for (int i = 0; i < children.size(); i++) {
//     pkb->insertRelation(RelationType::PARENT, parentIndex,
//                         std::to_string(children[i]->getStmtIndex());
//   }
// }
//
// void ParentExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
//   int parentIndex = node->getStmtIndex();
//   std::vector<std::shared_ptr<StmtNode>> children = node->getStmtLstNode()->getChildren();
//   for (int i = 0; i < children.size(); i++) {
//     pkb->insertRelation(RelationType::PARENT, parentIndex,
//                         std::to_string(children[i]->getStmtIndex());
//   }
// }

// void ParentExtractor::extractFromAssign(std::shared_ptr<AssignNode> node) {}
