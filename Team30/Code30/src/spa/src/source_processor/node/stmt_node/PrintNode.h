#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class PrintNode : public StmtNode {
public:
    explicit PrintNode(int stmtIndex, StmtType stmtType, std::string varName);

    ~PrintNode() = default;

private:
    const std::string varName;
};