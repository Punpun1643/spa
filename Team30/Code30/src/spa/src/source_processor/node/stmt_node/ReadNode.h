#pragma once

#include <string>

#include "StmtNode.h"
#include "StmtType.h"

class ReadNode : public StmtNode {
public:
    explicit ReadNode(int stmtIndex, StmtType stmtType, std::string varName);

    void accept(IDesignExtractor &designExtractor) override;

    const std::string &getVarName() const;

    ~ReadNode() = default;

private:
    const std::string varName;
};