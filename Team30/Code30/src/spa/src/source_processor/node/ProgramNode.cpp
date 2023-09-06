#include "ProgramNode.h"
#include "ProcedureNode.h"
#include "../design_extractor/IDesignExtractor.h"

// Constructs the ProgramNode
ProgramNode::ProgramNode(std::vector <std::shared_ptr<ProcedureNode>> procedures)
        : procedures(std::move(procedures)) {}

void ProgramNode::accept(IDesignExtractor &designExtractor) {
    auto thisProgramNodePtr = std::dynamic_pointer_cast<ProgramNode>(shared_from_this());
    designExtractor.extractFromProgram(thisProgramNodePtr);
};