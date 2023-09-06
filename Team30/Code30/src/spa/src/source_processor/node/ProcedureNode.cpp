#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(std::string procedureName, std::shared_ptr <StmtLstNode> stmtLstNode) : procedureName(
        procedureName), stmtLstNode(std::move(stmtLstNode)) {}