#include "ParentExtractor.h"

#include <iostream>
#include <vector>

ParentExtractor::ParentExtractor(PKBSPInterface& pkb)
    : pkb(pkb), DesignExtractor(pkb) {}

void ParentExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  std::vector<std::shared_ptr<StmtNode>> children =
      node->GetStmtLst()->GetChildren();
  for (int i = 0; i < children.size(); i++) {
    pkb.insertRelation(RelationType::PARENT,
                       std::to_string(node->GetStmtIndex()),
                       std::to_string(children[i]->GetStmtIndex()));
  }
}

void ParentExtractor::extractFromIf(std::shared_ptr<IfNode> node) {
  int parentIndex = node->GetStmtIndex();
  std::vector<std::shared_ptr<StmtNode>> children =
      node->GetThenStmtLst()->GetChildren();
  std::vector<std::shared_ptr<StmtNode>> elseChildren =
      node->GetElseStmtLst()->GetChildren();
  children.insert(std::end(children), std::begin(elseChildren),
                  std::end(elseChildren));
  for (int i = 0; i < children.size(); i++) {
    pkb.insertRelation(RelationType::PARENT,
                       std::to_string(node->GetStmtIndex()),
                       std::to_string(children[i]->GetStmtIndex()));
  }
}
