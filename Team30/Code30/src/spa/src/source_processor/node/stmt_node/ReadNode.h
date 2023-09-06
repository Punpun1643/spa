#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class ReadNode : public StmtNode {
public:
    ReadNode(int stmtIndex, StmtType stmtType, std::string varName);

    ~ReadNode() = default;

private:
    const std::string varName;
};