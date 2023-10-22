#include "IfNode.h"

#include <memory>
#include <utility>

#include "../../design_extractor/IDesignExtractor.h"

IfNode::IfNode(int stmt_index, StmtType stmt_type,
               std::shared_ptr<CondExprNode> cond_expr_node,
               std::shared_ptr<StmtLstNode> then_stmt_lst_node,
               std::shared_ptr<StmtLstNode> else_stmt_lst_node)
    : StmtNode(stmt_index, StmtType::IF_STMT),
      cond_expr_node(std::move(cond_expr_node)),
      then_stmt_lst_node(std::move(then_stmt_lst_node)),
      else_stmt_lst_node(std::move(else_stmt_lst_node)) {}

void IfNode::Accept(IDesignExtractor& design_extractor) {
  auto this_if_node_ptr = std::dynamic_pointer_cast<IfNode>(shared_from_this());
  design_extractor.ExtractFromIf(this_if_node_ptr);
}

std::shared_ptr<StmtLstNode> const IfNode::GetThenStmtLst() const {
  return then_stmt_lst_node;
}

std::shared_ptr<StmtLstNode> const IfNode::GetElseStmtLst() const {
  return else_stmt_lst_node;
}

std::shared_ptr<CondExprNode> const IfNode::GetCondExpr() const {
  return cond_expr_node;
}
