#include <utility>

#include "AssignNode.h"
#include "../../design_extractor/IDesignExtractor.h"

AssignNode::AssignNode(int stmtIndex, StmtType stmtType,
                       std::unordered_set<std::string> variables,
                       std::unordered_set<int> constants, std::string varName,
                       std::shared_ptr<TreeNode> exprTreeRoot)
    : StmtNode(stmtIndex, StmtType::ASSIGN_STMT),
      constants(
          std::make_shared<std::unordered_set<int>>(std::move(constants))),
      variables(std::make_shared<std::unordered_set<std::string>>(
          std::move(variables))),
      varName(varName),
      exprTreeRoot(exprTreeRoot) {}

std::string const& AssignNode::GetVarName() const {
  return varName;
}

std::shared_ptr<std::unordered_set<std::string>> AssignNode::GetVariables()
    const {
  return variables;
}

std::shared_ptr<std::unordered_set<int>> AssignNode::GetConstants() const {
  return constants;
}

std::shared_ptr<TreeNode> AssignNode::GetRootOfTree() const {
  return exprTreeRoot;
}

void AssignNode::Accept(IDesignExtractor& designExtractor) {
  auto thisAssignNodePtr =
      std::dynamic_pointer_cast<AssignNode>(shared_from_this());
  designExtractor.ExtractFromAssign(thisAssignNodePtr);
}
