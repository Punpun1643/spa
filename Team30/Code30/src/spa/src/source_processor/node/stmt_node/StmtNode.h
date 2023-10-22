#pragma once

#include "../ANode.h"
#include "StmtType.h"

class StmtNode : public ANode {
 public:
  explicit StmtNode(int stmt_index, StmtType stmt_type);

  int GetStmtIndex() const;

  StmtType GetStmtType() const;

  virtual ~StmtNode() = default;

 private:
  StmtType const stmt_type;
  int const stmt_index;
};
