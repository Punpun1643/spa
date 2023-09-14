#include "FollowsExtractor.h"

#include <program_knowledge_base/PkbApi.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKB.h"
#include "IDesignExtractor.h"

FollowsExtractor::FollowsExtractor(PkbApi& pkb) : pkb(pkb) {
  // TODO
}

void FollowsExtractor::extractFromProgram(std::shared_ptr<ProgramNode> node) {
  // TODO
}

void FollowsExtractor::extractFromProcedure(
    std::shared_ptr<ProcedureNode> node) {
  // TODO
}

void FollowsExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  std::vector<std::shared_ptr<StmtNode>> children = node->getChildren();
  for (int i = 0; i < children.size() - 1; i++) {
    pkb.insertRelation(RelationType::FOLLOWS, to_string(children[i]->getStmtIndex()),
                       to_string(children[i + 1]->getStmtIndex()));
  }
}

void FollowsExtractor::extractFromCall(std::shared_ptr<CallNode> node) {
  // TODO
}

void FollowsExtractor::extractFromPrint(std::shared_ptr<PrintNode> node) {
  // TODO
}

void FollowsExtractor::extractFromRead(std::shared_ptr<ReadNode> node) {
  // TODO
}
