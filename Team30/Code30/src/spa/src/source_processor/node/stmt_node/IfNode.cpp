#include "IfNode.h"

#include "../../design_extractor/IDesignExtractor.h"

IfNode::IfNode(int stmtIndex, StmtType stmtType,
               std::shared_ptr<CondExprNode> condExprNode,
               std::shared_ptr<StmtLstNode> thenStmtLstNode,
               std::shared_ptr<StmtLstNode> elseStmtLstNode)
    : StmtNode(stmtIndex, StmtType::IF_STMT),
      condExprNode(std::move(condExprNode)),
      thenStmtLstNode(std::move(thenStmtLstNode)),
      elseStmtLstNode(std::move(elseStmtLstNode)) {}

void IfNode::Accept(IDesignExtractor& designExtractor) {
  auto thisIfNodePtr = std::dynamic_pointer_cast<IfNode>(shared_from_this());
  designExtractor.ExtractFromIf(thisIfNodePtr);
}

std::shared_ptr<StmtLstNode> const IfNode::GetThenStmtLst() const {
  return thenStmtLstNode;
}

std::shared_ptr<StmtLstNode> const IfNode::GetElseStmtLst() const {
  return elseStmtLstNode;
}

std::shared_ptr<CondExprNode> const IfNode::GetCondExpr() const {
  return condExprNode;
}
