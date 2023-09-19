#include "AssignNode.h"

#include "../../design_extractor/IDesignExtractor.h"

AssignNode::AssignNode(int stmtIndex, StmtType stmtType,
                       std::unordered_set<std::string> variables,
                       std::unordered_set<int> constants, std::string varName,
                       std::shared_ptr<TreeNode> exprTreeRoot)
    : StmtNode(stmtIndex, StmtType::ASSIGN_STMT),
      constants(constants),
      variables(variables),
      varName(varName),
      exprTreeRoot(exprTreeRoot) {}

std::string const& AssignNode::getVarName() const { return varName; }

std::unordered_set<std::string> AssignNode::getVariables() const {
  return variables;
}

std::unordered_set<int> AssignNode::getConstants() const { return constants; }

int AssignNode::getStmtIndex() const { return stmtIndex; }

StmtType AssignNode::getStmtType() const { return stmtType; }

std::shared_ptr<TreeNode> AssignNode::getRootOfTree() const {
  return exprTreeRoot;
}

void AssignNode::accept(IDesignExtractor& designExtractor) {
  auto thisAssignNodePtr =
      std::dynamic_pointer_cast<AssignNode>(shared_from_this());
  designExtractor.extractFromAssign(thisAssignNodePtr);
}
