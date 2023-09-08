#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class CallNode : public StmtNode {
public:
    explicit CallNode(int stmtIndex, StmtType stmtType, std::string procName);

    void accept(IDesignExtractor &designExtractor) override;

    const std::string &getProcName() const;

    ~CallNode() = default;

private:
    const std::string procName;
};