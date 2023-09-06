#pragma once

#include "../ANode.h"
#include "StmtType.h"

class StmtNode : public ANode {
public:
    StmtNode(int stmtIndex, StmtType stmtType);

    virtual ~StmtNode() = default;

private:
    const int stmtIndex;
    const StmtType stmtType;
};