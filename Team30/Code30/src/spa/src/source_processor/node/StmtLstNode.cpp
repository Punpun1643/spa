#include <utility>

#include "StmtLstNode.h"
#include "../design_extractor/IDesignExtractor.h"

StmtLstNode::StmtLstNode(std::vector<std::shared_ptr<StmtNode>> stmts)
    : stmts(std::move(stmts)) {}

void StmtLstNode::Accept(IDesignExtractor& design_extractor) {
  auto this_stmt_lst_node_ptr =
      std::dynamic_pointer_cast<StmtLstNode>(shared_from_this());
  design_extractor.ExtractFromStmtLst(this_stmt_lst_node_ptr);
}

std::vector<std::shared_ptr<StmtNode>> StmtLstNode::GetChildren() const {
  return this->stmts;
}
