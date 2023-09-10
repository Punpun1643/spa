#include "ProgramNode.h"

#include "../design_extractor/IDesignExtractor.h"
#include "ProcedureNode.h"

// Constructs the ProgramNode
ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures)
    : procedures(std::move(procedures)) {}

void ProgramNode::accept(IDesignExtractor& designExtractor) {
  auto thisProgramNodePtr =
      std::dynamic_pointer_cast<ProgramNode>(shared_from_this());
  designExtractor.extractFromProgram(thisProgramNodePtr);
};

std::vector<std::shared_ptr<ProcedureNode>> ProgramNode::getChildren() const {
  return this->procedures;
}