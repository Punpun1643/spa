#include "WhileNode.h"

WhileNode::WhileNode(int stmtIndex, StmtType stmtType,
                     std::shared_ptr<CondExprNode> condExprNode,
                     std::shared_ptr<StmtLstNode> stmtLstNode)
    : StmtNode(stmtIndex, StmtType::WHILE_STMT),
      condExprNode(std::move(condExprNode)),
      stmtLstNode(std::move(stmtLstNode)) {}

std::shared_ptr<CondExprNode> WhileNode::getCondExpr() { return condExprNode; }

std::shared_ptr<StmtLstNode> WhileNode::getStmtLst() { return stmtLstNode; }