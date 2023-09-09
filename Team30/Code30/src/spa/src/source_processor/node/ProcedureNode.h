#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ANode.h"
#include "StmtLstNode.h"

class ProcedureNode : public ANode {
public:
    explicit ProcedureNode(std::string procedureName, std::shared_ptr <StmtLstNode> stmtLstNode);

    void accept(IDesignExtractor &designExtractor) override;

    std::vector <std::shared_ptr<StmtLstNode>> getChildren() const;

    ~ProcedureNode() = default;

private:
    const std::string procedureName;
    const std::shared_ptr <StmtLstNode> stmtLstNode;
};
