#pragma once

#include <memory>
#include <vector>

#include "ANode.h"
#include "ProcedureNode.h"

class ProgramNode : public ANode {
 public:
  explicit ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures);

  void Accept(IDesignExtractor& designExtractor) override;

  std::vector<std::shared_ptr<ProcedureNode>> GetChildren() const;

  ~ProgramNode() = default;

 private:
  std::vector<std::shared_ptr<ProcedureNode>> const procedures;
};
