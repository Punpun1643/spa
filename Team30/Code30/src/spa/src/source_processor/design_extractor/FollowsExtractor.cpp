#include "FollowsExtractor.h"

#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "../../program_knowledge_base/PKB.h"
#include "program_knowledge_base/PKBSPInterface.h"

FollowsExtractor::FollowsExtractor(PKBSPInterface& pkb)
    : pkb(pkb), DesignExtractor(pkb) {}

void FollowsExtractor::extractFromStmtLst(std::shared_ptr<StmtLstNode> node) {
  std::vector<std::shared_ptr<StmtNode>> children = node->GetChildren();
  for (int i = 0; i < children.size() - 1; i++) {
    pkb.insertRelation(RelationType::FOLLOWS,
                       std::to_string(children[i]->GetStmtIndex()),
                       std::to_string(children[i + 1]->GetStmtIndex()));
  }
}
