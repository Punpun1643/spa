#pragma once

#include <memory>
#include <vector>

#include "../node/ANode.h"
#include "IDesignExtractor.h"

class IExtractionController {
 public:
  virtual void ExecuteProgramExtraction(std::shared_ptr<ProgramNode> node) = 0;
  virtual void ExecuteProcedureExtraction(std::shared_ptr<ProcedureNode> node) = 0;
  virtual void ExecuteStmtLstExtraction(std::shared_ptr<StmtLstNode> node) = 0;
  virtual void ExecuteStmtExtraction(std::shared_ptr<StmtNode> node) = 0;

  virtual ~IExtractionController() = default;

 protected:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
};