#pragma once

#include <memory>

#include "../node/ProgramNode.h"
#include "../node/ProcedureNode.h"
#include "../node/StmtLstNode.h"

class IDesignExtractor {
public:
    virtual void extractFromProgram(std::shared_ptr <ProgramNode> node) = 0;

    virtual void extractFromProcedure(std::shared_ptr <ProcedureNode> node) = 0;

    virtual void extractFromStmtLst(std::shared_ptr <StmtLstNode> node) = 0;

    virtual ~IDesignExtractor() = default;
};