#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "../util_node/TreeNode.h"
#include "StmtNode.h"
#include "StmtType.h"

class AssignNode : public StmtNode {
 public:
  AssignNode(int stmtIndex, StmtType stmtType,
             std::unordered_set<std::string> variables,
             std::unordered_set<int> constants, std::string varName,
             std::shared_ptr<TreeNode> exprTreeRoot);

  std::unordered_set<std::string> getVariables() const;

  std::unordered_set<int> getConstants() const;

  std::string const& getVarName() const;

  std::shared_ptr<TreeNode> getRootOfTree() const;

  void accept(IDesignExtractor& designExtractor) override;

  ~AssignNode() = default;

 private:
  std::unordered_set<std::string> variables;
  std::unordered_set<int> constants;
  std::string varName;
  std::shared_ptr<TreeNode> exprTreeRoot;
};