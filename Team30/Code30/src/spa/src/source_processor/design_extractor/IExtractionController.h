#pragma once

#include <memory>
#include <vector>

#include "../node/ANode.h"
#include "IDesignExtractor.h"

class IExtractionController {
 public:
  virtual void executeProgramExtraction(std::shared_ptr<ProgramNode> node) = 0;
  virtual void executeProcedureExtraction(std::shared_ptr<ProcedureNode> node) = 0;
  virtual void executeStmtLstExtraction(std::shared_ptr<StmtLstNode> node) = 0;
  virtual void executeStmtExtraction(std::shared_ptr<StmtNode> node) = 0;

  virtual ~IExtractionController() = default;

 protected:
  std::vector<std::shared_ptr<IDesignExtractor>> extractors;
};