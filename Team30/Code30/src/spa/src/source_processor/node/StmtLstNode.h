#pragma once

#include <memory>
#include <vector>

#include "ANode.h"
#include "stmt_node/StmtNode.h"

class StmtLstNode : public ANode {
 public:
  explicit StmtLstNode(std::vector<std::shared_ptr<StmtNode>> stmts);

  void accept(IDesignExtractor& designExtractor) override;

  std::vector<std::shared_ptr<StmtNode>> getChildren() const;

  ~StmtLstNode() = default;

 private:
  std::vector<std::shared_ptr<StmtNode>> const stmts;
};