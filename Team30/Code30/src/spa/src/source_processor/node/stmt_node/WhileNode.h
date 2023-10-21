#pragma once

#include <memory>

#include "../StmtLstNode.h"
#include "../util_node/CondExprNode.h"
#include "StmtNode.h"
#include "StmtType.h"

class WhileNode : public StmtNode {
 public:
  explicit WhileNode(int stmtIndex, StmtType stmtType,
                     std::shared_ptr<CondExprNode> condExprNode,
                     std::shared_ptr<StmtLstNode> stmtLstNode);

  std::shared_ptr<CondExprNode> GetCondExpr();

  std::shared_ptr<StmtLstNode> GetStmtLst();

  void Accept(IDesignExtractor& designExtractor) override;

  ~WhileNode() = default;

 private:
  std::shared_ptr<CondExprNode> condExprNode;
  std::shared_ptr<StmtLstNode> stmtLstNode;
};
