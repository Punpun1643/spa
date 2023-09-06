#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ANode.h"
#include "StmtLstNode.h"

class ProcedureNode : public ANode {
public:
    explicit ProcedureNode(std::string procedureName, std::shared_ptr <StmtLstNode> stmtLstNode);

    ~ProcedureNode() = default;

private:
    std::string procedureName;
    std::shared_ptr <StmtLstNode> stmtLstNode;
};
