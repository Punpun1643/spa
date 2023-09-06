#pragma once

#include <memory>
#include <vector>

#include "ANode.h"
#include "stmt_node/StmtNode.h"

class StmtLstNode : public ANode {
public:
    explicit StmtLstNode(std::vector <std::shared_ptr<StmtNode>> stmts);

    void accept(IDesignExtractor &designExtractor) override;

    ~StmtLstNode() = default;

private:
    const std::vector <std::shared_ptr<StmtNode>> stmts;
};