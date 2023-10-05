#include "FollowsExtractor.h"

#include "program_knowledge_base/PKBSPInterface.h"
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKB.h"

FollowsExtractor::FollowsExtractor(PkbApi& pkb) : pkb(pkb), DesignExtractor(pkb) {}

void FollowsExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  std::vector<std::shared_ptr<StmtNode>> children = node->getChildren();
  for (int i = 0; i < children.size() - 1; i++) {
    pkb.insertRelation(RelationType::FOLLOWS,
                       std::to_string(children[i]->getStmtIndex()),
                       std::to_string(children[i + 1]->getStmtIndex()));
  }
}
