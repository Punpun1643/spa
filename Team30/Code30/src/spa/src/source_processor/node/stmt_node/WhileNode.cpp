#include <utility>

#include "WhileNode.h"
#include "../../design_extractor/IDesignExtractor.h"

WhileNode::WhileNode(int stmt_index, StmtType stmt_type,
                     std::shared_ptr<CondExprNode> cond_expr_node,
                     std::shared_ptr<StmtLstNode> stmt_lst_node)
    : StmtNode(stmt_index, StmtType::WHILE_STMT),
      cond_expr_node(std::move(cond_expr_node)),
      stmt_lst_node(std::move(stmt_lst_node)) {}

void WhileNode::Accept(IDesignExtractor& design_extractor) {
  auto this_while_node_ptr =
      std::dynamic_pointer_cast<WhileNode>(shared_from_this());
  design_extractor.ExtractFromWhile(this_while_node_ptr);
}

std::shared_ptr<CondExprNode> WhileNode::GetCondExpr() {
  return cond_expr_node;
}

std::shared_ptr<StmtLstNode> WhileNode::GetStmtLst() {
  return stmt_lst_node;
}
