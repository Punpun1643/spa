#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class CallNode : public StmtNode {
public:
    explicit CallNode(int stmtIndex, StmtType stmtType, std::string procName);

    ~CallNode() = default;

private:
    const std::string procName;
};