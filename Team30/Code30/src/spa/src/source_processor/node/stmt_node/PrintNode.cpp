#include "PrintNode.h"

#include "../../design_extractor/IDesignExtractor.h"

PrintNode::PrintNode(int stmt_index, StmtType stmt_type, std::string var_name)
    : StmtNode(stmt_index, StmtType::PRINT_STMT), var_name(var_name) {}

void PrintNode::Accept(IDesignExtractor& design_extractor) {
  auto this_print_node_ptr =
      std::dynamic_pointer_cast<PrintNode>(shared_from_this());
  design_extractor.ExtractFromPrint(this_print_node_ptr);
}

std::string const& PrintNode::GetVarName() const {
  return var_name;
}
