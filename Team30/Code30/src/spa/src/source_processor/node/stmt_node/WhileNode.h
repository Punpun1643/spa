#pragma once

#include <memory>

#include "../StmtLstNode.h"
#include "../util_node/CondExprNode.h"
#include "StmtNode.h"
#include "StmtType.h"

class WhileNode : public StmtNode {
 public:
  explicit WhileNode(int stmt_index, StmtType stmt_type,
                     std::shared_ptr<CondExprNode> cond_expr_node,
                     std::shared_ptr<StmtLstNode> stmt_lst_node);

  std::shared_ptr<CondExprNode> GetCondExpr();

  std::shared_ptr<StmtLstNode> GetStmtLst();

  void Accept(IDesignExtractor& design_extractor) override;

  ~WhileNode() = default;

 private:
  std::shared_ptr<CondExprNode> cond_expr_node;
  std::shared_ptr<StmtLstNode> stmt_lst_node;
};
