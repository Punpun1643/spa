#include "StmtLstNode.h"

#include "../design_extractor/IDesignExtractor.h"

StmtLstNode::StmtLstNode(std::vector<std::shared_ptr<StmtNode>> stmts)
    : stmts(std::move(stmts)) {}

void StmtLstNode::accept(IDesignExtractor& designExtractor) {
  auto thisStmtLstNodePtr =
      std::dynamic_pointer_cast<StmtLstNode>(shared_from_this());
  designExtractor.ExtractFromStmtLst(thisStmtLstNodePtr);
}

std::vector<std::shared_ptr<StmtNode>> StmtLstNode::getChildren() const {
  return this->stmts;
}
