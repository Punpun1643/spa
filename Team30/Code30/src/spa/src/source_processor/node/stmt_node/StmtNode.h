#pragma once

#include "../ANode.h"
#include "StmtType.h"

class StmtNode : public ANode {
public:
    explicit StmtNode(int stmtIndex, StmtType stmtType);

    const StmtType stmtType; // for testing only

    virtual ~StmtNode() = default;

private:
    const int stmtIndex;
//    const StmtType stmtType;
};