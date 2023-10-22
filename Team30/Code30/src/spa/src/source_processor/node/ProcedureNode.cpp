#include "ProcedureNode.h"

#include <utility>

#include "../design_extractor/IDesignExtractor.h"

ProcedureNode::ProcedureNode(std::string procedure_name,
                             std::shared_ptr<StmtLstNode> stmt_lst_node)
    : procedure_name(procedure_name), stmt_lst_node(std::move(stmt_lst_node)) {}

void ProcedureNode::Accept(IDesignExtractor& design_extractor) {
  auto this_procedure_node_ptr =
      std::dynamic_pointer_cast<ProcedureNode>(shared_from_this());
  design_extractor.ExtractFromProcedure(this_procedure_node_ptr);
}

std::vector<std::shared_ptr<StmtLstNode>> ProcedureNode::GetChildren() const {
  return {this->stmt_lst_node};
}

std::string ProcedureNode::GetProcedureName() const {
  return this->procedure_name;
}
