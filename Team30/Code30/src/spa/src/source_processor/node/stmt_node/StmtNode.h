#pragma once

#include "../ANode.h"
#include "StmtType.h"

class StmtNode : public ANode {
 public:
  explicit StmtNode(int stmtIndex, StmtType stmtType);

  int getStmtIndex() const;

  StmtType getStmtType() const;

  virtual ~StmtNode() = default;

 private:
  StmtType const stmtType;
  int const stmtIndex;
};