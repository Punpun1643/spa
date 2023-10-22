
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ANode.h"
#include "StmtLstNode.h"

class ProcedureNode : public ANode {
 public:
  explicit ProcedureNode(std::string procedure_name,
                         std::shared_ptr<StmtLstNode> stmt_lst_node);

  void Accept(IDesignExtractor& design_extractor) override;

  std::vector<std::shared_ptr<StmtLstNode>> GetChildren() const;

  std::string GetProcedureName() const;

  ~ProcedureNode() = default;

 private:
  std::string const procedure_name;
  std::shared_ptr<StmtLstNode> const stmt_lst_node;
};
