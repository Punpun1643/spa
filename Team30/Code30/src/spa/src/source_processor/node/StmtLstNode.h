#pragma once

#include <memory>
#include <vector>

#include "ANode.h"
#include "stmt_node/StmtNode.h"

class StmtLstNode : public ANode {
public:
    explicit StmtLstNode(std::vector <std::shared_ptr<StmtNode>> stmts);

    ~StmtLstNode() = default;

private:

};