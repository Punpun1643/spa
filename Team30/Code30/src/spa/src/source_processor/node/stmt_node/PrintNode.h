#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class PrintNode : public StmtNode {
public:
    explicit PrintNode(int stmtIndex, StmtType stmtType, std::string varName);

    void accept(IDesignExtractor &designExtractor) override;

    const std::string varName; // for testing only

    ~PrintNode() = default;

private:
//    const std::string varName;
};