#include "ReadNode.h"

#include "../../design_extractor/IDesignExtractor.h"

ReadNode::ReadNode(int stmt_index, StmtType stmt_type, std::string var_name)
    : StmtNode(stmt_index, StmtType::READ_STMT), var_name(var_name) {}

void ReadNode::Accept(IDesignExtractor& design_extractor) {
  auto this_read_node_ptr =
      std::dynamic_pointer_cast<ReadNode>(shared_from_this());
  design_extractor.ExtractFromRead(this_read_node_ptr);
}

std::string const& ReadNode::GetVarName() const {
  return var_name;
}
