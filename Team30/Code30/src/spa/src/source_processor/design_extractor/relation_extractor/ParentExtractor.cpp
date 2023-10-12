#include "ParentExtractor.h"

#include <iostream>
#include <vector>

ParentExtractor::ParentExtractor(PKBSPInterface& pkb)
    : pkb(pkb), DesignExtractor(pkb) {}

void ParentExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  std::vector<std::shared_ptr<StmtNode>> children =
      node->getStmtLst()->getChildren();
  for (int i = 0; i < children.size(); i++) {
    pkb.insertRelation(RelationType::PARENT,
                       std::to_string(node->getStmtIndex()),
                       std::to_string(children[i]->getStmtIndex()));
  }
}

void ParentExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
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
