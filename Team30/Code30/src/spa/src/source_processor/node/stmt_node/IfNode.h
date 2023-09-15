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

  void accept(IDesignExtractor& extractor) override;

  std::shared_ptr<StmtLstNode> const getThenStmtLstNode() const;
  std::shared_ptr<StmtLstNode> const getElseStmtLstNode() const;
  std::shared_ptr<CondExprNode> const getCondExprNode() const;

 private:
  std::shared_ptr<StmtLstNode> const thenStmtLstNode;
  std::shared_ptr<StmtLstNode> const elseStmtLstNode;
  std::shared_ptr<CondExprNode> const condExprNode;
};