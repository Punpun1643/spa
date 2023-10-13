
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ANode.h"
#include "StmtLstNode.h"

class ProcedureNode : public ANode {
 public:
  explicit ProcedureNode(std::string procedureName,
                         std::shared_ptr<StmtLstNode> stmtLstNode);

  void Accept(IDesignExtractor& designExtractor) override;

  std::vector<std::shared_ptr<StmtLstNode>> GetChildren() const;

  std::string GetProcedureName() const;

  ~ProcedureNode() = default;

 private:
  std::string const procedureName;
  std::shared_ptr<StmtLstNode> const stmtLstNode;
};