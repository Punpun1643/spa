#pragma once

#include "../ANode.h"
#include "StmtType.h"

class StmtNode : public ANode {
 public:
  explicit StmtNode(int stmtIndex, StmtType stmtType);

  int GetStmtIndex() const;

  StmtType GetStmtType() const;

  virtual ~StmtNode() = default;

 private:
  StmtType const stmtType;
  int const stmtIndex;
};