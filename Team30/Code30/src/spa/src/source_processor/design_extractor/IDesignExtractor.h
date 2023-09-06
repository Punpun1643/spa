#pragma once

#include <memory>

#include "../node/ProgramNode.h"
#include "../node/ProcedureNode.h"
#include "../node/StmtLstNode.h"
#include "../node/stmt_node/CallNode.h"
#include "../node/stmt_node/PrintNode.h"
#include "../node/stmt_node/ReadNode.h"

class IDesignExtractor {
public:
    virtual void extractFromProgram(std::shared_ptr <ProgramNode> node) = 0;

    virtual void extractFromProcedure(std::shared_ptr <ProcedureNode> node) = 0;

    virtual void extractFromStmtLst(std::shared_ptr <StmtLstNode> node) = 0;

    virtual void extractFromCall(std::shared_ptr <CallNode> node) = 0;

    virtual void extractFromPrint(std::shared_ptr <PrintNode> node) = 0;

    virtual void extractFromRead(std::shared_ptr <ReadNode> node) = 0;

    virtual ~IDesignExtractor() = default;
};