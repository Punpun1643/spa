#include "ParentExtractor.h"

#include <iostream>
#include <vector>

ParentExtractor::ParentExtractor(PKBSPInterface& pkb)
    : pkb(pkb), DesignExtractor(pkb) {}

void ParentExtractor::ExtractFromWhile(std::shared_ptr<WhileNode> node) {
  std::vector<std::shared_ptr<StmtNode>> children =
      node->GetStmtLst()->GetChildren();
  for (int i = 0; i < children.size(); i++) {
    pkb.InsertRelation(RelationType::PARENT,
                       std::to_string(node->GetStmtIndex()),
                       std::to_string(children[i]->GetStmtIndex()));
  }
}

void ParentExtractor::ExtractFromIf(std::shared_ptr<IfNode> node) {
  int parent_index = node->GetStmtIndex();
  std::vector<std::shared_ptr<StmtNode>> children =
      node->GetThenStmtLst()->GetChildren();
  std::vector<std::shared_ptr<StmtNode>> else_children =
      node->GetElseStmtLst()->GetChildren();
  children.insert(std::end(children), std::begin(else_children),
                  std::end(else_children));
  for (int i = 0; i < children.size(); i++) {
    pkb.InsertRelation(RelationType::PARENT,
                       std::to_string(node->GetStmtIndex()),
                       std::to_string(children[i]->GetStmtIndex()));
  }
}
