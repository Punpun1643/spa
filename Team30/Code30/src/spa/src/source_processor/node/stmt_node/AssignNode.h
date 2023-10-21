#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "../../../shared/parser/node/TreeNode.h"
#include "StmtNode.h"
#include "StmtType.h"

class AssignNode : public StmtNode {
 public:
  AssignNode(int stmtIndex, StmtType stmtType,
             std::unordered_set<std::string> variables,
             std::unordered_set<int> constants, std::string varName,
             std::shared_ptr<TreeNode> exprTreeRoot);

  std::shared_ptr<std::unordered_set<std::string>> GetVariables() const;

  std::shared_ptr<std::unordered_set<int>> GetConstants() const;

  std::string const& GetVarName() const;

  std::shared_ptr<TreeNode> GetRootOfTree() const;

  void Accept(IDesignExtractor& designExtractor) override;

  ~AssignNode() = default;

 private:
  std::shared_ptr<std::unordered_set<std::string>> variables;
  std::shared_ptr<std::unordered_set<int>> constants;
  std::string varName;
  std::shared_ptr<TreeNode> exprTreeRoot;
};
