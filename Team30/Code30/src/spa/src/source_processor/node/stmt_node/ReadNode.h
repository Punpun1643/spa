#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class ReadNode : public StmtNode {
public:
    explicit ReadNode(int stmtIndex, StmtType stmtType, std::string varName);

    void accept(IDesignExtractor &designExtractor) override;

    ~ReadNode() = default;

private:
    const std::string varName;
};