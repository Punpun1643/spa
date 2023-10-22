#include "AssignNode.h"

#include <utility>

#include "../../design_extractor/IDesignExtractor.h"

AssignNode::AssignNode(int stmt_index, StmtType stmt_type,
                       std::unordered_set<std::string> variables,
                       std::unordered_set<int> constants, std::string var_name,
                       std::shared_ptr<TreeNode> expr_tree_root)
    : StmtNode(stmt_index, StmtType::ASSIGN_STMT),
      constants(
          std::make_shared<std::unordered_set<int>>(std::move(constants))),
      variables(std::make_shared<std::unordered_set<std::string>>(
          std::move(variables))),
      var_name(var_name),
      expr_tree_root(expr_tree_root) {}

std::string const& AssignNode::GetVarName() const {
  return var_name;
}

std::shared_ptr<std::unordered_set<std::string>> AssignNode::GetVariables()
    const {
  return variables;
}

std::shared_ptr<std::unordered_set<int>> AssignNode::GetConstants() const {
  return constants;
}

std::shared_ptr<TreeNode> AssignNode::GetRootOfTree() const {
  return expr_tree_root;
}

void AssignNode::Accept(IDesignExtractor& design_extractor) {
  auto this_assign_node_ptr =
      std::dynamic_pointer_cast<AssignNode>(shared_from_this());
  design_extractor.ExtractFromAssign(this_assign_node_ptr);
}
