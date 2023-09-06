#include "ProcedureNode.h"
#include "../design_extractor/IDesignExtractor.h"

ProcedureNode::ProcedureNode(std::string procedureName, std::shared_ptr <StmtLstNode> stmtLstNode) : procedureName(
        procedureName), stmtLstNode(std::move(stmtLstNode)) {}


void ProcedureNode::accept(IDesignExtractor &designExtractor) {
    auto thisProcedureNodePtr = std::dynamic_pointer_cast<ProcedureNode>(shared_from_this());
    designExtractor.extractFromProcedure(thisProcedureNodePtr);
}