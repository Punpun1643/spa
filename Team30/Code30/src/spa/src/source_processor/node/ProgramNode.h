#pragma once

#include <memory>
#include <vector>

#include "ANode.h"
#include "ProcedureNode.h"

class ProgramNode : public ANode {
 public:
  explicit ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures);

  void accept(IDesignExtractor& designExtractor) override;

  std::vector<std::shared_ptr<ProcedureNode>> getChildren() const;

  ~ProgramNode() = default;

 private:
  std::vector<std::shared_ptr<ProcedureNode>> const procedures;
};