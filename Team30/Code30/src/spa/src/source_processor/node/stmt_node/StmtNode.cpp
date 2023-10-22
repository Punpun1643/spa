#include "StmtNode.h"

StmtNode::StmtNode(int stmt_index, StmtType stmt_type)
    : stmt_index(stmt_index), stmt_type(stmt_type) {}

int StmtNode::GetStmtIndex() const {
  return stmt_index;
}

StmtType StmtNode::GetStmtType() const {
  return stmt_type;
}
