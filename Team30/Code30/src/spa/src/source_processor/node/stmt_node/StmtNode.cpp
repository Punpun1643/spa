#include "StmtNode.h"

StmtNode::StmtNode(int stmtIndex, StmtType stmtType)
    : stmtIndex(stmtIndex), stmtType(stmtType) {}

int StmtNode::getStmtIndex() const { return stmtIndex; }

StmtType StmtNode::getStmtType() const { return stmtType; }
