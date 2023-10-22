#include "CallNode.h"

#include "../../design_extractor/IDesignExtractor.h"

CallNode::CallNode(int stmt_index, StmtType stmt_type, std::string proc_name)
    : StmtNode(stmt_index, StmtType::CALL_STMT), proc_name(proc_name) {}

void CallNode::Accept(IDesignExtractor& design_extractor) {
  auto this_call_node_ptr =
      std::dynamic_pointer_cast<CallNode>(shared_from_this());
  design_extractor.ExtractFromCall(this_call_node_ptr);
}

std::string const& CallNode::GetProcName() const {
  return proc_name;
}
