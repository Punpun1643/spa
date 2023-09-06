#include "ProgramNode.h"
#include "ProcedureNode.h"

// Constructs the ProgramNode
ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures)
    : procedures(std::move(procedures)) {}