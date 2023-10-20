#include "WhileNode.h"

#include "../../design_extractor/IDesignExtractor.h"

WhileNode::WhileNode(int stmtIndex, StmtType stmtType,
                     std::shared_ptr<CondExprNode> condExprNode,
                     std::shared_ptr<StmtLstNode> stmtLstNode)
    : StmtNode(stmtIndex, StmtType::WHILE_STMT),
      condExprNode(std::move(condExprNode)),
      stmtLstNode(std::move(stmtLstNode)) {}

void WhileNode::Accept(IDesignExtractor& designExtractor) {
  auto thisWhileNodePtr =
      std::dynamic_pointer_cast<WhileNode>(shared_from_this());
  designExtractor.extractFromWhile(thisWhileNodePtr);
}

std::shared_ptr<CondExprNode> WhileNode::GetCondExpr() {
  return condExprNode;
}

std::shared_ptr<StmtLstNode> WhileNode::GetStmtLst() {
  return stmtLstNode;
}
