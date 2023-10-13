#pragma once

#include "../StmtLstNode.h"
#include "../util_node/CondExprNode.h"
#include "StmtNode.h"
#include "StmtType.h"

class IfNode : public StmtNode {
 public:
  IfNode(int stmtIndex, StmtType stmtType,
         std::shared_ptr<CondExprNode> condExprNode,
         std::shared_ptr<StmtLstNode> thenStmtLstNode,
         std::shared_ptr<StmtLstNode> elseStmtLstNode);

  void Accept(IDesignExtractor& extractor) override;

  std::shared_ptr<StmtLstNode> const GetThenStmtLst() const;
  std::shared_ptr<StmtLstNode> const GetElseStmtLst() const;
  std::shared_ptr<CondExprNode> const GetCondExpr() const;

  ~IfNode() = default;

 private:
  std::shared_ptr<StmtLstNode> const thenStmtLstNode;
  std::shared_ptr<StmtLstNode> const elseStmtLstNode;
  std::shared_ptr<CondExprNode> const condExprNode;
};