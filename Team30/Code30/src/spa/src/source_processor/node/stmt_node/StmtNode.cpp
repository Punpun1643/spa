#include "StmtNode.h"

StmtNode::StmtNode(int stmtIndex, StmtType stmtType)
    : stmtIndex(stmtIndex), stmtType(stmtType) {}

int StmtNode::GetStmtIndex() const { return stmtIndex; }

StmtType StmtNode::GetStmtType() const { return stmtType; }
