#pragma once

#include <memory>

#include "../StmtLstNode.h"
#include "../util_node/CondExprNode.h"
#include "StmtNode.h"
#include "StmtType.h"

class IfNode : public StmtNode {
 public:
  IfNode(int stmt_index, StmtType stmt_type,
         std::shared_ptr<CondExprNode> cond_expr_node,
         std::shared_ptr<StmtLstNode> then_stmt_lst_node,
         std::shared_ptr<StmtLstNode> else_stmt_lst_node);

  void Accept(IDesignExtractor& extractor) override;

  std::shared_ptr<StmtLstNode> const GetThenStmtLst() const;
  std::shared_ptr<StmtLstNode> const GetElseStmtLst() const;
  std::shared_ptr<CondExprNode> const GetCondExpr() const;

  ~IfNode() = default;

 private:
  std::shared_ptr<StmtLstNode> const then_stmt_lst_node;
  std::shared_ptr<StmtLstNode> const else_stmt_lst_node;
  std::shared_ptr<CondExprNode> const cond_expr_node;
};
