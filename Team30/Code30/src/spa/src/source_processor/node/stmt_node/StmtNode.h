#pragma once

#include "../ANode.h"
#include "StmtType.h"

class StmtNode : public ANode {
public:
    explicit StmtNode(int stmtIndex, StmtType stmtType);

    const StmtType stmtType; // for testing only

    int getStmtIndex() const;

    virtual ~StmtNode() = default;

private:
//    const StmtType stmtType;
    const int stmtIndex;
};